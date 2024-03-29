#include <MKTAppEngine.h>

void _VE_RUN_recordCommandBuffer(VkCommandBuffer commandBuffer, unsigned int imageIndex)
{
    RDEBUG("II run:recordCommandBuffer II");

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _renderPass;
    renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset.x = 0;
    renderPassInfo.renderArea.offset.y = 0;
    renderPassInfo.renderArea.extent = _swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkDeviceSize tempOffset = 0;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)_swapChainExtent.width;
        viewport.height = (float)_swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent = _swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        // for(IntDex i = 0; i < _ren_sizeOfAG; i++)
        // {

        //     vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[0].graphicsPipeline);

        //     vkCmdBindIndexBuffer(commandBuffer, _ren_AG[i].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        //     vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_ren_AG[i].vertexBuffer, &tempOffset);

        //     vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[0].pipelineLayout, 0, 1, &_ren_AG[i].descriptorSets[_currentFrame], 0, NULL);

        //     vkCmdDrawIndexed(commandBuffer, _ren_AG[i].sizeOfIndices, 1, 0, 0, 0);

        // }
        // printf("sizes: %d %d %d\n",_ren_sizeOfPiC,_ren_sizeOfAG,_ren_sizeOfBM);
        for(IntDex i = 0; i < _ren_sizeOfPiC; i++)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[1].graphicsPipeline);

            vkCmdBindIndexBuffer(commandBuffer,_ren_PiC[i].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindVertexBuffers(commandBuffer, 0, 1, & _ren_PiC[i].vertexBuffer, &tempOffset);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[1].pipelineLayout, 0, 1, &_ren_PiC[i].descriptorSets[_currentFrame], 0, NULL);

            vkCmdDrawIndexed(commandBuffer, _ren_PiC[i]._dataPiC.sizeOfIndices, 1, 0, 0, 0);

        }
        // for(IntDex i = 0; i < _ren_sizeOfBM; i++)
        // {
        //     vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[2].graphicsPipeline);

        //     vkCmdBindIndexBuffer(commandBuffer,_ren_BM[i].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        //     vkCmdBindVertexBuffers(commandBuffer, 0, 1, & _ren_BM[i].vertexBuffer, &tempOffset);

        //     vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[2].pipelineLayout, 0, 1, &_ren_BM[i].descriptorSets[_currentFrame], 0, NULL);

        //     vkCmdDrawIndexed(commandBuffer, _ren_BM[i]._dataPiC.sizeOfIndices, 1, 0, 0, 0);

        // }
        // for(IntDex i = 0; i < _ren_sizeOfBM; i++)
        // {
        //     vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[3].graphicsPipeline);

        //     vkCmdBindIndexBuffer(commandBuffer,_ren_BM[i].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        //     vkCmdBindVertexBuffers(commandBuffer, 0, 1, & _ren_BM[i].vertexBuffer, &tempOffset);

        //     vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[3].pipelineLayout, 0, 1, &_ren_BM[i].descriptorSets[_currentFrame], 0, NULL);

        //     vkCmdDrawIndexed(commandBuffer, _ren_BM[i]._dataPiC.sizeOfIndices, 1, 0, 0, 0);

        // }
        // for(IntDex i = 0; i < _ren_sizeOfQDR; i++)
        // {
        //     vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[3].graphicsPipeline);

        //     vkCmdBindIndexBuffer(commandBuffer,_ren_QDR[i].indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        //     vkCmdBindVertexBuffers(commandBuffer, 0, 1, & _ren_QDR[i].vertexBuffer, &tempOffset);

        //     vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _ren_materials[3].pipelineLayout, 0, 1, &_ren_QDR[i].descriptorSets[_currentFrame], 0, NULL);

        //     vkCmdDrawIndexed(commandBuffer, _ren_QDR[i]._dataQDR.sizeOfIndices, 1, 0, 0, 0);

        // }

    vkCmdEndRenderPass(commandBuffer);

    VK_CHECK(vkEndCommandBuffer(commandBuffer));
    RDEBUG("II> run:recordCommandBuffer <II");
    return;
}

void _VE_RUN_drawFrame()
{
    vkWaitForFences(_device, 1, &_runtimeKit[_currentFrame].inFlightFence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _runtimeKit[_currentFrame].imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebufferResized) {
        _framebufferResized = 0;
        _VE_UTILS_RecreateSwapChain();
        return;
    } else if (result != VK_SUCCESS) {
        printf("failed to acquire swap chain image!\n");
    }

    vkResetFences(_device, 1, &_runtimeKit[_currentFrame].inFlightFence);

    vkResetCommandBuffer(_commandBuffers[_currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    _VE_RUN_recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);
    
    RDEBUG("test1234");




    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[1] = {_runtimeKit[_currentFrame].imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

    VkSemaphore signalSemaphores[1] = {_runtimeKit[_currentFrame].renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    RDEBUG("test12345");

    VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _runtimeKit[_currentFrame].inFlightFence));
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[1] = {_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(_presentQueue, &presentInfo);

    RDEBUG("test12346");
    _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    return;
}

void _VE_RUN_updateDescriptors(void * data, IntDex dataSize, IntDex type, IntDex index)
{
    void* Odata;
    if(type == 0)
    {
        for(int i = 0; i < _ren_AG[index].sizeOfUniformBuffers;i++)
        {
            vkMapMemory(_device, _ren_AG[index].uniformBuffersMemory[i], 0, dataSize, 0, &Odata);
            for(IntDex i = 0; i < dataSize;i++)
                *(char *)(Odata+i) = *(char *)(data+i);
            vkUnmapMemory(_device, _ren_AG[index].uniformBuffersMemory[i]);
        }
    } else if(type == 3)
    {
        for(int i = 0; i < _ren_QDR[index].sizeOfUniformBuffers;i++)
        {
            vkMapMemory(_device, _ren_QDR[index].uniformBuffersMemory[i], 0, dataSize, 0, &Odata);
            for(IntDex i = 0; i < dataSize;i++)
                *(char *)(Odata+i) = *(char *)(data+i);
            vkUnmapMemory(_device, _ren_AG[index].uniformBuffersMemory[i]);
        }
    }
    return;
}