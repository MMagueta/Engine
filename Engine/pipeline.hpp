#pragma once

#include "device.hpp"
#include "vulkan/vulkan_core.h"

//#include <_types/_uint32_t.h>
#include <string>
#include <vector>

namespace engine {
    struct PipelineConfiguration {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline {
        public:
        Pipeline(const std::string& vertFilePath, const std::string& fragFilePath, Device&, const PipelineConfiguration&);

        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        void operator=(const Pipeline&) = delete;

        static PipelineConfiguration defaultPipelineConfiguration(uint32_t width, uint32_t height);

        void bind(VkCommandBuffer);

        private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfiguration&);

        void createShaderModule(const std::vector<char>&, VkShaderModule*);

        Device & device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}