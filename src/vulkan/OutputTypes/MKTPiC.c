int loading_MKTP_image(MKTPic data)
{
    VkDeviceSize imageSize = data.width * data.height * 4;

    // createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkBufferCreateInfo stagingBufferInfo = {};
	stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingBufferInfo.pNext = nullptr;
	stagingBufferInfo.size = imageSize;
	stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

    void* data_ptr;
    vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data_ptr);
    memcpy(data_ptr, data.pixels, static_cast<size_t>(imageSize)); // now let us just hope that my made up format will work in this highly delicate system
    vkUnmapMemory(_device, stagingBufferMemory);
    // vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data_ptr);
    // memcpy(data_ptr, data.pixels, static_cast<size_t>(imageSize)); // now let us just hope that my made up format will work in this highly delicate system
    // vkUnmapMemory(_device, stagingBufferMemory);
    DEBUG("mapped memory");

    

    free(data.pixels); // #ff0000 might cause some minor fatal errors

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(data.width);
    imageInfo.extent.height = static_cast<uint32_t>(data.height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional
    DEBUG("created image info");
    VK_CHECK(vkCreateImage(_device, &imageInfo, nullptr, &textureImage));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, textureImage, &memRequirements);
    DEBUG("got image memory requirements");
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    DEBUG("found memory type");

    VK_CHECK(vkAllocateMemory(_device, &allocInfo, nullptr, &textureImageMemory));
    DEBUG("allocated memory");

    vkBindImageMemory(_device, textureImage, textureImageMemory, 0);

///later
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(data.width), static_cast<uint32_t>(data.height));

    DEBUG("copied buffer to image");
    transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    // vkFreeMemory(_device, stagingBufferMemory, nullptr); // #ff0000

    VkImageView textureImageView;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(_device, &viewInfo, nullptr, &textureImageView));
    DEBUG("created image view");


    _pictures.resize(_pictures.size()+1);
    imagesLoaded++;
    _pictures[_pictures.size()-1] = {textureImage,textureImageMemory,textureImageView,_defaultPushConstants,1};

    _pictures[_pictures.size()-1].PC.transformation = {0.1,0.0,0.0,0.0,
                                    0.0,0.1,0.0,0.0,
                                    0.0,0.0,0.1,0.0,
                                    0.0,0.0,0.0,1.0};


    CreatePictureDescriptors(_pictures.data(),_pictures.size()-1);
    addToRenderArray(&_render,1, _pictures.size()-1, 0);

    _mainDeletionQueue.push_function([=]() {
        vkDestroyImageView(_device, _pictures[_pictures.size()-1].textureImageView, nullptr);
        vkDestroyImage(_device, _pictures[_pictures.size()-1].textureImage, nullptr);
        vkFreeMemory(_device, _pictures[_pictures.size()-1].textureImageMemory, nullptr);
    });
    DEBUG("loaded the logo");
    return _pictures.size()-1;
    // some shananigans
}