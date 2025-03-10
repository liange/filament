/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// No include guard!!!

/*
 * How to use this header file.
 *
 * The DECL_DRIVER_API() macro must be defined before including this file:
 *     DECL_DRIVER_API(method_name, params_decl, params)
 *
 *     method_name          the name of the method (e.g. beginRenderPass)
 *     params_decl          the list of typed params
 *     params               the parameters, without their type (e.g.: for a function call)
 *
 * The following example adds all the driver methods as virtual
 * members of class:
 *
 * class MyDriver : public Driver {
 * public:
 * #define DRIVER_API(method_name, params_decl, params) \
 *     virtual void method_name(params_decl) override
 * #include "DriverAPI.h"
 * };
 *
 * DECL_DRIVER_API is automatically undefined.
 */

#ifndef DECL_DRIVER_API
#error "The DECL_DRIVER_API() macro must be defined before including this file"
#define DECL_DRIVER_API(M, D, P)
#endif

#ifndef DECL_DRIVER_API_SYNCHRONOUS
#error "The DECL_DRIVER_API_SYNCHRONOUS() macro must be defined before including this file"
#define DECL_DRIVER_API_SYNCHRONOUS(R, M, D, P)
#endif

#ifndef DECL_DRIVER_API_RETURN
#error "The DECL_DRIVER_API_RETURN() macro must be defined before including this file"
#define DECL_DRIVER_API_RETURN(R, M, D, P)
#endif


/*
 * Convenience macros. These are PRIVATE, don't use.
 */

#define EXPAND(x) x

#define PAIR_ARGS_0(M, ...)
#define PAIR_ARGS_1(M, X, Y, ...) M(X, Y)
#define PAIR_ARGS_2(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_1(M, __VA_ARGS__))
#define PAIR_ARGS_3(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_2(M, __VA_ARGS__))
#define PAIR_ARGS_4(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_3(M, __VA_ARGS__))
#define PAIR_ARGS_5(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_4(M, __VA_ARGS__))
#define PAIR_ARGS_6(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_5(M, __VA_ARGS__))
#define PAIR_ARGS_7(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_6(M, __VA_ARGS__))
#define PAIR_ARGS_8(M, X, Y, ...) M(X, Y), EXPAND(PAIR_ARGS_7(M, __VA_ARGS__))

#define PAIR_ARGS_N__(_0, E1, _1, E2, _2, E3, _3, E4, _4, E5, _5, E6, _6, E7, _7, E8, _8, X, ...) PAIR_ARGS_##X

#define PAIR_ARGS_N(M, ...) \
    EXPAND(EXPAND(PAIR_ARGS_N__(0, ##__VA_ARGS__, 8, E, 7, E, 6, E, 5, E, 4, E, 3, E, 2, E, 1, E, 0))(M, __VA_ARGS__))

#define ARG(T, P) T P

#define PARAM(T, P) P

#define DECL_DRIVER_API_N(N, ...) \
    DECL_DRIVER_API(N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_R_N(R, N, ...) \
    DECL_DRIVER_API_RETURN(R, N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

#define DECL_DRIVER_API_SYNCHRONOUS_N(R, N, ...) \
    DECL_DRIVER_API_SYNCHRONOUS(R, N, PAIR_ARGS_N(ARG, ##__VA_ARGS__), PAIR_ARGS_N(PARAM, ##__VA_ARGS__))

// on some compilers the ##__VA_ARGS__ hack is not supported, so we can't handle 0-parameter APIs
// with DECL_DRIVER_API_SYNCHRONOUS_N
#define DECL_DRIVER_API_SYNCHRONOUS_0(R, N) \
    DECL_DRIVER_API_SYNCHRONOUS(R, N,,)

// we can't handle empty lists because we don't have __VA_OPT__() in C++14 and the ## hack
// only works for omitted arguments, not empty lists
#define PARAM_1( P, ...) P
#define ARG_1( T, P, ...) T P
#define DECL_DRIVER_API_0(N)      DECL_DRIVER_API(N, ARG_1(int, dummy=0), PARAM_1(dummy))
#define DECL_DRIVER_API_R_0(R, N) DECL_DRIVER_API_RETURN(R, N, ARG_1(int, dummy=0), PARAM_1(dummy))

/*
 * Driver API below...
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

DECL_DRIVER_API_N(beginFrame,
        int64_t, monotonic_clock_ns,
        uint32_t, frameId,
        backend::FrameFinishedCallback, callback,
        void*, user)

DECL_DRIVER_API_N(setPresentationTime,
        int64_t, monotonic_clock_ns)

DECL_DRIVER_API_N(endFrame,
        uint32_t, frameId)

// hint to the driver that we're done with all render targets up to this point. i.e. the driver
// can start rendering. e.g. correspond to glFlush() for a GLES driver.
DECL_DRIVER_API_0(flush)

// flush and wait for the effects to be done
DECL_DRIVER_API_0(finish)

/*
 * Creating driver objects
 * -----------------------
 */

DECL_DRIVER_API_R_N(backend::VertexBufferHandle, createVertexBuffer,
        uint8_t, bufferCount,
        uint8_t, attributeCount,
        uint32_t, vertexCount,
        backend::AttributeArray, attributes,
        backend::BufferUsage, usage)

DECL_DRIVER_API_R_N(backend::IndexBufferHandle, createIndexBuffer,
        backend::ElementType, elementType,
        uint32_t, indexCount,
        backend::BufferUsage, usage)

DECL_DRIVER_API_R_N(backend::TextureHandle, createTexture,
        backend::SamplerType, target,
        uint8_t, levels,
        backend::TextureFormat, format,
        uint8_t, samples,
        uint32_t, width,
        uint32_t, height,
        uint32_t, depth,
        backend::TextureUsage, usage)

DECL_DRIVER_API_R_N(backend::SamplerGroupHandle, createSamplerGroup,
        size_t, size)

DECL_DRIVER_API_R_N(backend::UniformBufferHandle, createUniformBuffer,
        size_t, size,
        backend::BufferUsage, usage)

DECL_DRIVER_API_R_0(backend::RenderPrimitiveHandle, createRenderPrimitive)

DECL_DRIVER_API_R_N(backend::ProgramHandle, createProgram,
        backend::Program&&, program)

DECL_DRIVER_API_R_0(backend::RenderTargetHandle, createDefaultRenderTarget)

DECL_DRIVER_API_R_N(backend::RenderTargetHandle, createRenderTarget,
        backend::TargetBufferFlags, targetBufferFlags,
        uint32_t, width,
        uint32_t, height,
        uint8_t, samples,
        backend::TargetBufferInfo, color,
        backend::TargetBufferInfo, depth,
        backend::TargetBufferInfo, stencil)

DECL_DRIVER_API_R_0(backend::FenceHandle, createFence)

DECL_DRIVER_API_R_N(backend::SwapChainHandle, createSwapChain,
        void*, nativeWindow,
        uint64_t, flags)

DECL_DRIVER_API_R_N(backend::SwapChainHandle, createSwapChainHeadless,
        uint32_t, width,
        uint32_t, height,
        uint64_t, flags)

DECL_DRIVER_API_R_N(backend::StreamHandle, createStreamFromTextureId,
        intptr_t, externalTextureId,
        uint32_t, width,
        uint32_t, height)

/*
 * Destroying driver objects
 * -------------------------
 */

DECL_DRIVER_API_N(destroyVertexBuffer,    backend::VertexBufferHandle, vbh)
DECL_DRIVER_API_N(destroyIndexBuffer,     backend::IndexBufferHandle, ibh)
DECL_DRIVER_API_N(destroyRenderPrimitive, backend::RenderPrimitiveHandle, rph)
DECL_DRIVER_API_N(destroyProgram,         backend::ProgramHandle, ph)
DECL_DRIVER_API_N(destroySamplerGroup,    backend::SamplerGroupHandle, sbh)
DECL_DRIVER_API_N(destroyUniformBuffer,   backend::UniformBufferHandle, ubh)
DECL_DRIVER_API_N(destroyTexture,         backend::TextureHandle, th)
DECL_DRIVER_API_N(destroyRenderTarget,    backend::RenderTargetHandle, rth)
DECL_DRIVER_API_N(destroySwapChain,       backend::SwapChainHandle, sch)
DECL_DRIVER_API_N(destroyStream,          backend::StreamHandle, sh)

/*
 * Synchronous APIs
 * ----------------
 */

DECL_DRIVER_API_SYNCHRONOUS_0(void, terminate)
DECL_DRIVER_API_SYNCHRONOUS_N(backend::StreamHandle, createStreamNative, void*, stream)
DECL_DRIVER_API_SYNCHRONOUS_0(backend::StreamHandle, createStreamAcquired)
DECL_DRIVER_API_SYNCHRONOUS_N(void, setAcquiredImage, backend::StreamHandle, stream, void*, image, backend::StreamCallback, cb, void*, userData)
DECL_DRIVER_API_SYNCHRONOUS_N(void, setStreamDimensions, backend::StreamHandle, stream, uint32_t, width, uint32_t, height)
DECL_DRIVER_API_SYNCHRONOUS_N(int64_t, getStreamTimestamp, backend::StreamHandle, stream)
DECL_DRIVER_API_SYNCHRONOUS_N(void, updateStreams, backend::DriverApi*, driver)
DECL_DRIVER_API_SYNCHRONOUS_N(void, destroyFence, backend::FenceHandle, fh)
DECL_DRIVER_API_SYNCHRONOUS_N(backend::FenceStatus, wait, backend::FenceHandle, fh, uint64_t, timeout)
DECL_DRIVER_API_SYNCHRONOUS_N(bool, isTextureFormatSupported, backend::TextureFormat, format)
DECL_DRIVER_API_SYNCHRONOUS_N(bool, isTextureFormatMipmappable, backend::TextureFormat, format)
DECL_DRIVER_API_SYNCHRONOUS_N(bool, isRenderTargetFormatSupported, backend::TextureFormat, format)
DECL_DRIVER_API_SYNCHRONOUS_0(bool, isFrameTimeSupported)
DECL_DRIVER_API_SYNCHRONOUS_0(bool, canGenerateMipmaps)
DECL_DRIVER_API_SYNCHRONOUS_N(void, setupExternalImage, void*, image)
DECL_DRIVER_API_SYNCHRONOUS_N(void, cancelExternalImage, void*, image)

/*
 * Updating driver objects
 * -----------------------
 */

DECL_DRIVER_API_N(updateVertexBuffer,
        backend::VertexBufferHandle, vbh,
        size_t, index,
        backend::BufferDescriptor&&, data,
        uint32_t, byteOffset)

DECL_DRIVER_API_N(updateIndexBuffer,
        backend::IndexBufferHandle, ibh,
        backend::BufferDescriptor&&, data,
        uint32_t, byteOffset)

DECL_DRIVER_API_N(loadUniformBuffer,
        backend::UniformBufferHandle, ubh,
        backend::BufferDescriptor&&, buffer)

DECL_DRIVER_API_N(updateSamplerGroup,
        backend::SamplerGroupHandle, ubh,
        backend::SamplerGroup&&, samplerGroup)

DECL_DRIVER_API_N(update2DImage,
        backend::TextureHandle, th,
        uint32_t, level,
        uint32_t, xoffset,
        uint32_t, yoffset,
        uint32_t, width,
        uint32_t, height,
        backend::PixelBufferDescriptor&&, data)

DECL_DRIVER_API_N(updateCubeImage,
        backend::TextureHandle, th,
        uint32_t, level,
        backend::PixelBufferDescriptor&&, data,
        backend::FaceOffsets, faceOffsets)

DECL_DRIVER_API_N(generateMipmaps,
        backend::TextureHandle, th)

DECL_DRIVER_API_N(setExternalImage,
        backend::TextureHandle, th,
        void*, image)

DECL_DRIVER_API_N(setExternalStream,
        backend::TextureHandle, th,
        backend::StreamHandle, sh)

DECL_DRIVER_API_N(beginRenderPass,
        backend::RenderTargetHandle, rth,
        const backend::RenderPassParams&, params)

DECL_DRIVER_API_0(endRenderPass)

DECL_DRIVER_API_N(discardSubRenderTargetBuffers,
        backend::RenderTargetHandle, rth,
        backend::TargetBufferFlags, targetBufferFlags,
        uint32_t, left,
        uint32_t, bottom,
        uint32_t, width,
        uint32_t, height)

DECL_DRIVER_API_N(setRenderPrimitiveBuffer,
        backend::RenderPrimitiveHandle, rph,
        backend::VertexBufferHandle, vbh,
        backend::IndexBufferHandle, ibh,
        uint32_t, enabledAttributes)

DECL_DRIVER_API_N(setRenderPrimitiveRange,
        backend::RenderPrimitiveHandle, rph,
        backend::PrimitiveType, pt,
        uint32_t, offset,
        uint32_t, minIndex,
        uint32_t, maxIndex,
        uint32_t, count)

/*
 * Swap chain
 */

DECL_DRIVER_API_N(makeCurrent,
        backend::SwapChainHandle, schDraw,
        backend::SwapChainHandle, schRead)


DECL_DRIVER_API_N(commit,
        backend::SwapChainHandle, sch)

/*
 * Setting rendering state
 * -----------------------
 */

DECL_DRIVER_API_N(bindUniformBuffer,
        size_t, index,
        backend::UniformBufferHandle, ubh)

DECL_DRIVER_API_N(bindUniformBufferRange,
        size_t, index,
        backend::UniformBufferHandle, ubh,
        size_t, offset,
        size_t, size)

DECL_DRIVER_API_N(bindSamplers,
        size_t, index,
        backend::SamplerGroupHandle, sbh)

DECL_DRIVER_API_N(insertEventMarker,
        const char*, string,
        size_t, len = 0)

DECL_DRIVER_API_N(pushGroupMarker,
        const char*, string,
        size_t, len = 0)

DECL_DRIVER_API_0(popGroupMarker)

DECL_DRIVER_API_0(startCapture)

DECL_DRIVER_API_0(stopCapture)


/*
 * Read-back operations
 * --------------------
 */

DECL_DRIVER_API_N(readPixels,
        backend::RenderTargetHandle, src,
        uint32_t, x,
        uint32_t, y,
        uint32_t, width,
        uint32_t, height,
        backend::PixelBufferDescriptor&&, data)

DECL_DRIVER_API_N(readStreamPixels,
        backend::StreamHandle, sh,
        uint32_t, x,
        uint32_t, y,
        uint32_t, width,
        uint32_t, height,
        backend::PixelBufferDescriptor&&, data)

/*
 * Rendering operations
 * --------------------
 */

DECL_DRIVER_API_N(blit,
        backend::TargetBufferFlags, buffers,
        backend::RenderTargetHandle, dst,
        backend::Viewport, dstRect,
        backend::RenderTargetHandle, src,
        backend::Viewport, srcRect,
        backend::SamplerMagFilter, filter)

DECL_DRIVER_API_N(draw,
        backend::PipelineState, state,
        backend::RenderPrimitiveHandle, rph)

#pragma clang diagnostic pop

#undef EXPAND

#undef PARAM_1
#undef ARG_1
#undef DECL_DRIVER_API_0
#undef DECL_DRIVER_API_R_0

#undef PARAM
#undef ARG
#undef DECL_DRIVER_API_N
#undef DECL_DRIVER_API_R_N
#undef DECL_DRIVER_API_SYNCHRONOUS_N
#undef DECL_DRIVER_API_SYNCHRONOUS_0

#undef DECL_DRIVER_API
#undef DECL_DRIVER_API_SYNCHRONOUS
#undef DECL_DRIVER_API_RETURN

#undef PAIR_ARGS_1
#undef PAIR_ARGS_2
#undef PAIR_ARGS_3
#undef PAIR_ARGS_4
#undef PAIR_ARGS_5
#undef PAIR_ARGS_6
#undef PAIR_ARGS_7
#undef PAIR_ARGS_8
#undef PAIR_ARGS_N__
#undef PAIR_ARGS_N

// No include guard!!!
