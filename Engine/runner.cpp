#include "runner.hpp"
#include <stdexcept>

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
            createPipeline();
            createCommandBuffers();
        }
        Runner::~Runner() {
            vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
        }

        void Runner::loadModels() {
            std::vector<engine::Model::Vertex> vertices{
                { {0.0f, -0.5f}},
                  {{0.5f, 0.5f}},
                  {{-0.5f, 0.5f}} };
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
        void Runner::createPipeline() {
            auto pipelineConfig = engine::Pipeline::defaultPipelineConfiguration(swapChain.width(), swapChain.height());
            pipelineConfig.renderPass = swapChain.getRenderPass();
            pipelineConfig.pipelineLayout = pipelineLayout;
            pipeline = std::make_unique<engine::Pipeline>(
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.vert.svt",
                "C:/Users/mague/Source/Repos/Engine/Engine/Shaders/simple_shader.frag.svt",
                device,
                pipelineConfig
            );
        }
        void Runner::createCommandBuffers() {
            commandBuffers.resize(swapChain.imageCount());
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = device.getCommandPool();
            allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
            if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
                throw std::runtime_error("Failed to allocate command buffers.");

            for (int i = 0; i < commandBuffers.size(); i++) {
                VkCommandBufferBeginInfo beginInfo{};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
                    throw std::runtime_error("Failed to begin recording command buffer.");

                VkRenderPassBeginInfo renderPassInfo{};

                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassInfo.renderPass = swapChain.getRenderPass();
                renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);
                renderPassInfo.renderArea.offset = { 0, 0 };
                renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

                std::array<VkClearValue, 2> clearValues{};
                // 0 is the color attachment
                clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
                // 1 is the depth attachment on the framebuffer
                clearValues[1].depthStencil = { 1.0f, 0 };

                renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
                renderPassInfo.pClearValues = clearValues.data();

                vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                pipeline->bind(commandBuffers[i]);
                model->bind(commandBuffers[i]);
                model->draw(commandBuffers[i]);

                vkCmdEndRenderPass(commandBuffers[i]);
                if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
                    throw std::runtime_error("Failed to end render pass.");
            }
        }
        void Runner::drawFrame() {
            uint32_t imageIndex;
            VkResult result = swapChain.acquireNextImage(&imageIndex);
            if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
                throw std::runtime_error("Failed to acquire swap chain image.");

            result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
            if (result != VK_SUCCESS)
                throw std::runtime_error("Failed to display swap chain image.");
        }
}