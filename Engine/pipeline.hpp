#pragma once

#include "device.hpp"
#include "vulkan/vulkan_core.h"

//#include <_types/_uint32_t.h>
#include <string>
#include <vector>

namespace engine {
    struct PipelineConfiguration {
        PipelineConfiguration(const PipelineConfiguration&) = delete;
        PipelineConfiguration& operator=(const PipelineConfiguration&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline {
        public:
        Pipeline(const std::string& vertFilePath, const std::string& fragFilePath, Device&, const PipelineConfiguration&);

        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline operator=(const Pipeline&) = delete;

        static void defaultPipelineConfiguration(PipelineConfiguration&);

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