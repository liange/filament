/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <jni.h>

#include <gltfio/FilamentAsset.h>

using namespace filament;
using namespace filament::math;
using namespace gltfio;
using namespace utils;

extern "C" JNIEXPORT jint JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetRoot(JNIEnv*, jclass, jlong nativeAsset) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    return asset->getRoot().getId();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetEntityCount(JNIEnv*, jclass,
        jlong nativeAsset) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    return asset->getEntityCount();
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetEntities(JNIEnv* env, jclass,
        jlong nativeAsset, jintArray result) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    Entity* entities = (Entity*) env->GetIntArrayElements(result, nullptr);
    std::copy_n(asset->getEntities(), asset->getEntityCount(), entities);
    env->ReleaseIntArrayElements(result, (jint*) entities, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetBoundingBox(JNIEnv* env, jclass,
        jlong nativeAsset, jfloatArray result) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    float* values = env->GetFloatArrayElements(result, nullptr);
    const filament::Aabb box = asset->getBoundingBox();
    const float3 center = box.center();
    const float3 extent = box.extent();
    values[0] = center.x;
    values[1] = center.y;
    values[2] = center.z;
    values[3] = extent.x;
    values[4] = extent.y;
    values[5] = extent.z;
    env->ReleaseFloatArrayElements(result, values, 0);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetName(JNIEnv* env, jclass,
        jlong nativeAsset, jint entityId) {
    uint32_t id = static_cast<uint32_t>(entityId);
    Entity* entity = (Entity*) &id;
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    const char* val = asset->getName(*entity);
    return val ? env->NewStringUTF(val) : nullptr;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetAnimator(JNIEnv* , jclass,
        jlong nativeAsset) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    return (jlong) asset->getAnimator();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetResourceUriCount(JNIEnv*, jclass,
                                                                           jlong nativeAsset) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    return (jint) asset->getResourceUriCount();
}

extern "C" JNIEXPORT void JNICALL
Java_com_google_android_filament_gltfio_FilamentAsset_nGetResourceUris(JNIEnv* env, jclass,
                                                                       jlong nativeAsset,
                                                                       jobjectArray result) {
    FilamentAsset* asset = (FilamentAsset*) nativeAsset;
    auto resourceUris = asset->getResourceUris();
    for (int i = 0; i < asset->getResourceUriCount(); ++i) {
        env->SetObjectArrayElement(result, (jsize) i, env->NewStringUTF(resourceUris[i]));
    }
}
