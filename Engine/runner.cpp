#include "runner.hpp"
#include <stdexcept>
#include <cassert>

namespace engine {

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void Runner::run() {
            while (!window.shouldClose()) {
                glfwPollEvents();
                drawFrame();
            }

            vkDeviceWaitIdle(device.device());
        };

        Runner::Runner() {
            loadModels();
            createPipelineLayout();
            recreateSwapChain();
            createCommandBuffers();
        }
        Runner::~Runner() {
            vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
        }

        void Runner::loadModels() {
            std::vector<engine::Model::Vertex> vertices{
                { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                { { 0.5f, 0.5f  }, { 0.0f, 1.0f, 0.0f } },
                { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } } };
            model = std::make_unique<engine::Model>(device, vertices);
            return;
        }

        void Runner::createPipelineLayout() {
            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 0;
            pipelineLayoutInfo.pSetLayouts = nullptr;
            pipelineLayoutInfo.pushConstantRangeCount = 0;
            pipelineLayoutInfo.pPushConstantRanges = nullptr;
            if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
                throw std::exception("Failed to create pipeline layout.");
        }
        /*
        void Runner::createPipeline() {
            auto pipelineConfig = engine::Pipeline::defaultPipelineConfiguration(swapChain->width(), swapChain->height());
            pipelineConfig.renderPass = swapChain->getRenderPass();
            pipelineConfig.pipelineLayout = pipelineLayout;
            pipeline = std::make_unique<engine::Pipeline>(
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
                device,
                pipelineConfig
            );
        }
        */

        void Runner::createPipeline() {
            assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
            assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

            PipelineConfiguration pipelineConfig{};
            Pipeline::defaultPipelineConfiguration(pipelineConfig);
            pipelineConfig.renderPass = swapChain->getRenderPass();
            pipelineConfig.pipelineLayout = pipelineLayout;
            pipeline = std::make_unique<engine::Pipeline>(
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
                device,
                pipelineConfig
            );
        }

        void Runner::recreateSwapChain() {
            auto extent = window.getExtent();
            while (extent.width == 0 || extent.height == 0) {
                extent = window.getExtent();
                glfwWaitEvents();
            }
            vkDeviceWaitIdle(device.device());

            if (swapChain == nullptr) {
                swapChain = std::make_unique<SwapChain>(device, extent);
            }
            else {
                swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
                if (swapChain->imageCount() != commandBuffers.size()) {
                    freeCommandBuffers();
                    createCommandBuffers();
                }
            }

            createPipeline();
        }

        void Runner::createCommandBuffers() {
            commandBuffers.resize(swapChain->imageCount());
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = device.getCommandPool();
            allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
            if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
                throw std::runtime_error("Failed to allocate command buffers.");
        }

        void Runner::freeCommandBuffers() {
            vkFreeCommandBuffers(
                device.device(),
                device.getCommandPool(),
                static_cast<uint32_t>(commandBuffers.size()),
                commandBuffers.data());
            commandBuffers.clear();
        }

        void Runner::recordCommandBuffer(int imageIndex) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain->getRenderPass();
            renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
            viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            VkRect2D scissor{ {0, 0}, swapChain->getSwapChainExtent() };
            vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
            vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

            pipeline->bind(commandBuffers[imageIndex]);
            model->bind(commandBuffers[imageIndex]);
            model->draw(commandBuffers[imageIndex]);

            vkCmdEndRenderPass(commandBuffers[imageIndex]);
            if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }

        void Runner::drawFrame() {
            uint32_t imageIndex;
            auto result = swapChain->acquireNextImage(&imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                recreateSwapChain();
                return;
            }

            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("Failed to acquire swap chain image.");
            }

            recordCommandBuffer(imageIndex);
            result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
                window.wasWindowResized()) {
                window.resetWindowResizedFlag();
                recreateSwapChain();
                return;
            }
            else if (result != VK_SUCCESS) {
                throw std::runtime_error("Failed to display swap chain image.");
            }
        }
}