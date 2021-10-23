#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <VapourSynth4.h>
#include <VSHelper4.h>

typedef struct {
    int w,h;
    VSVideoInfo *vi;
    float *mat;
    FILE* logf;
} mat_data;

float * fetch_mat(const char* fn, int* w, int* h) {

    FILE* fp = fopen(fn, "r");
    
    fscanf(fp, "%d %d\n",w,h);

    float * mat = (float*)malloc(*w*(*h)*sizeof(float));
    int i;

    for(i = 0; i < *w*(*h); i++)
        fscanf(fp, "%f ",mat+i);

    return mat;
}

static const VSFrame *VS_CC matGetFrame(int n, int activationReason, void *instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
    mat_data *d = (mat_data *)instanceData;
// fprintf(d->logf,"%d",activationReason == arAllFramesReady);
// fflush(d->logf);
    // if (activationReason == arAllFramesReady) {

    VSFrame *dst = vsapi->newVideoFrame(&(d -> vi -> format), d -> w, d -> h, NULL, core);
    VSVideoFormat fi = d->vi->format;
// fprintf(d->logf,"%d %d",fi.bitsPerSample,fi.bytesPerSample);
// fflush(d->logf);
    int plane = 0;

    float *dstp = (float*)vsapi->getWritePtr(dst, plane);
    int dst_stride = vsapi->getStride(dst, plane); 
    int h = vsapi->getFrameHeight(dst, plane);
    int y;
    int w = vsapi->getFrameWidth(dst, plane);
    int x;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++)
            dstp[x] = d->mat[y*w+x];

        dstp += dst_stride/fi.bytesPerSample;
    }

    for (plane = 1; plane < fi.numPlanes; plane++) {
        float *dstp = (float*)vsapi->getWritePtr(dst, plane);
        int dst_stride = vsapi->getStride(dst, plane); // note that if a frame has the same dimensions and format, the stride is guaranteed to be the same. int dst_stride = src_stride would be fine too in this filter.
        // Since planes may be subsampled you have to query the height of them individually
        int h = vsapi->getFrameHeight(dst, plane);
        int y;
        int w = vsapi->getFrameWidth(dst, plane);
        int x;

        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++)
                dstp[x] = 0;

            dstp += dst_stride/fi.bytesPerSample;
        }
    }    
    (void)activationReason;
    (void)frameData;
    (void)frameCtx;
    (void)n;


    return dst;
    // }

}

// Free all allocated data on filter destruction
static void VS_CC matFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
    mat_data *d = (mat_data *)instanceData;
    free(d->vi);
    free(d->mat);
    free(d);

    (void)core;
    (void)vsapi;
}

// This function is responsible for validating arguments and creating a new filter
static void VS_CC matCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
    mat_data d;
    mat_data *data;

    d.mat = fetch_mat(vsapi->mapGetData(in, "matfile", 0, 0),&d.h, &d.w);


    VSVideoInfo *vi = malloc(sizeof(VSVideoInfo));
    vi -> format . colorFamily = cfYUV; //only work with grey in this demo
    vi -> format . sampleType = 1;
    vi -> format . bitsPerSample = sizeof(float)*8;
    vi -> format . bytesPerSample = sizeof(float);
    vi -> format . subSamplingW = 0;
    vi -> format . subSamplingH = 0;
    vi -> format . numPlanes = 3;

    vi -> fpsNum = 1;
    vi -> fpsDen = 1;
    vi -> width = d.w;
    vi -> height = d.h;
    vi -> numFrames = 1;

    d.vi = vi;

    data = (mat_data *)malloc(sizeof(d));
    *data = d;

    // data -> logf = fopen("C:\\Users\\gabri\\workzone\\C++\\vapoursynth\\ptt challeneg\\src\\log.txt","w");
// fprintf(data->logf,"create%d %d\n",d.w,d.h);
// fflush(data->logf);
    // VSFilterDependency deps[] = {{NULL,0}};
    vsapi->createVideoFilter(out, "matread", vi, matGetFrame, matFree, fmParallel, NULL, 0, data, core);
;
    (void)userData;

}


VS_EXTERNAL_API(void) VapourSynthPluginInit2(VSPlugin *plugin, const VSPLUGINAPI *vspapi) {
    vspapi->configPlugin("com.vapoursynth.mat", "MAT", "VapourSynth Invert Example", VS_MAKE_VERSION(1, 0), VAPOURSYNTH_API_VERSION, 0, plugin);
    vspapi->registerFunction("matread", "matfile:data", "clip:vnode;", matCreate, NULL, plugin);
}
