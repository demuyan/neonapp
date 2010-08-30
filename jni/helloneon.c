#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpu-features.h>


#define DEBUG 0

#if DEBUG
#include <android/log.h>
#  define  D(x...)  __android_log_print(ANDROID_LOG_INFO,"helloneon",x)
#else
#  define  D(...)  do {} while (0)
#endif

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-neon/project/src/com/example/neon/HelloNeon.java
 */
jstring
Java_com_example_neon_HelloNeon_stringFromJNI( JNIEnv* env,
                                               jobject thiz )
{
    char*  str;
    uint64_t features;
    char buffer[512]="";
    
    strcpy(buffer, "");
    if (android_getCpuFamily() != ANDROID_CPU_FAMILY_ARM) {
        strlcat(buffer, "Not an ARM CPU !\n", sizeof buffer);
        goto EXIT;
    }

    features = android_getCpuFeatures();
    if ((features & ANDROID_CPU_ARM_FEATURE_ARMv7) == 0) {
        strlcat(buffer, "Not an ARMv7 CPU !\n", sizeof buffer);
        goto EXIT;
    }

    /* HAVE_NEON is defined in Android.mk ! */
#ifdef HAVE_NEON
    if ((features & ANDROID_CPU_ARM_FEATURE_NEON) == 0) {
        strlcat(buffer, "CPU doesn't support NEON !\n", sizeof buffer);
        goto EXIT;
    }

    const float mat1[] = {10,20,30,40, 10,10,10,10, 10,10,10,10, 10,10,10,10};
    const float mat2[] = {11,12,13,14, 10,10,10,10, 10,10,10,10, 10,10,10,10};
    float outmat[16];

    multMatrix(mat1, mat2, outmat);

    asprintf(&str, "outmat[] = {%5.0f,%5.0f,%5.0f,%5.0f,  %5.0f,%5.0f,%5.0f,%5.0f,  %5.0f,%5.0f,%5.0f,%5.0f,  %5.0f,%5.0f,%5.0f,%5.0f}\n", outmat[0],outmat[1],outmat[2],outmat[3],outmat[4],outmat[5],outmat[6],outmat[7],outmat[8],outmat[9],outmat[10],outmat[11],outmat[12],outmat[13],outmat[14],outmat[15]);
    strlcat(buffer, str, sizeof buffer);
    free(str);
    
#else /* !HAVE_NEON */
    strlcat(buffer, "Program not compiled with ARMv7 support !\n", sizeof buffer);
#endif /* !HAVE_NEON */
EXIT:
    return (*env)->NewStringUTF(env, buffer);
}
