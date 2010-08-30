#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpu-features.h>
#include <arm_neon.h>

#define DEBUG 0
// #define NEON_INLINE

#if DEBUG
#include <android/log.h>
#  define  D(x...)  __android_log_print(ANDROID_LOG_INFO,"helloneon",x)
#else
#  define  D(...)  do {} while (0)
#endif

void
multMatrix(float *mat1, float *mat2, float *outmat)
{

#ifdef NEON_INLINE

	__asm__ volatile
	        (
	         "vldmia %1, { q4-q7 }  \n\t"
	         "vldmia %2, { q8-q11 } \n\t"

	         "vmul.f32 q0, q8, d8[0]\n\t"
	         "vmul.f32 q1, q8, d10[0]\n\t"
	         "vmul.f32 q2, q8, d12[0]\n\t"
	         "vmul.f32 q3, q8, d14[0]\n\t"

	         "vmla.f32 q0, q9, d8[1]\n\t"
	         "vmla.f32 q1, q9, d10[1]\n\t"
	         "vmla.f32 q2, q9, d12[1]\n\t"
	         "vmla.f32 q3, q9, d14[1]\n\t"

	         "vmla.f32 q0, q10, d9[0]\n\t"
	         "vmla.f32 q1, q10, d11[0]\n\t"
	         "vmla.f32 q2, q10, d13[0]\n\t"
	         "vmla.f32 q3, q10, d15[0]\n\t"

	         "vmla.f32 q0, q11, d9[1]\n\t"
	         "vmla.f32 q1, q11, d11[1]\n\t"
	         "vmla.f32 q2, q11, d13[1]\n\t"
	         "vmla.f32 q3, q11, d15[1]\n\t"

	         "vstmia %0, { q0-q3 }"
	         : // no output
	         : "r" (outmat), "r" (mat1), "r" (mat2)
	         : "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q11" 
	         );
#else

    float32x4_t u0,u1,u2,u3;
    float32x4_t v0,v1,v2,v3;

    v0 = vld1q_f32(&mat1[0]);
    v1 = vld1q_f32(&mat1[4]);
    v2 = vld1q_f32(&mat1[8]);
    v3 = vld1q_f32(&mat1[12]);

    u0 = vld1q_f32(&mat2[0]);
    u1 = vld1q_f32(&mat2[4]);
    u2 = vld1q_f32(&mat2[8]);
    u3 = vld1q_f32(&mat2[12]);

    const float32x4_t A1 = vmulq_f32(u0, vdupq_n_f32(vgetq_lane_f32(v0,0)));
    const float32x4_t B1 = vmulq_f32(u1, vdupq_n_f32(vgetq_lane_f32(v0,1)));
    const float32x4_t C1 = vmulq_f32(u2, vdupq_n_f32(vgetq_lane_f32(v0,2)));
    const float32x4_t D1 = vmulq_f32(u3, vdupq_n_f32(vgetq_lane_f32(v0,3)));
    const float32x4_t _v0 = vaddq_f32( vaddq_f32(A1 , B1), vaddq_f32(C1,  D1) );

    const float32x4_t A2 = vmulq_f32(u0, vdupq_n_f32(vgetq_lane_f32(v1,0)));
    const float32x4_t B2 = vmulq_f32(u1, vdupq_n_f32(vgetq_lane_f32(v1,1)));
    const float32x4_t C2 = vmulq_f32(u2, vdupq_n_f32(vgetq_lane_f32(v1,2)));
    const float32x4_t D2 = vmulq_f32(u3, vdupq_n_f32(vgetq_lane_f32(v1,3)));
    const float32x4_t _v1 = vaddq_f32( vaddq_f32(A2 , B2),  vaddq_f32(C2 ,  D2) );

    const float32x4_t A3 = vmulq_f32(u0, vdupq_n_f32(vgetq_lane_f32(v2,0)));
    const float32x4_t B3 = vmulq_f32(u1, vdupq_n_f32(vgetq_lane_f32(v2,1)));
    const float32x4_t C3 = vmulq_f32(u2, vdupq_n_f32(vgetq_lane_f32(v2,2)));
    const float32x4_t D3 = vmulq_f32(u3, vdupq_n_f32(vgetq_lane_f32(v2,3)));
    const float32x4_t _v2 = vaddq_f32( vaddq_f32(A3 , B3),  vaddq_f32(C3 ,  D3) );

    const float32x4_t A4 = vmulq_f32(u0, vdupq_n_f32(vgetq_lane_f32(v3,0)));
    const float32x4_t B4 = vmulq_f32(u1, vdupq_n_f32(vgetq_lane_f32(v3,1)));
    const float32x4_t C4 = vmulq_f32(u2, vdupq_n_f32(vgetq_lane_f32(v3,2)));
    const float32x4_t D4 = vmulq_f32(u3, vdupq_n_f32(vgetq_lane_f32(v3,3)));
    const float32x4_t _v3 = vaddq_f32( vaddq_f32(A4 , B4),  vaddq_f32(C4,  D4) );

    vst1q_f32(&outmat[0], _v0);
    vst1q_f32(&outmat[4], _v1);
    vst1q_f32(&outmat[8], _v2);
    vst1q_f32(&outmat[12], _v3);

#endif

}
