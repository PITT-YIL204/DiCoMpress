#include "VSScript4.h"
#include "VSHelper4.h"
#include <stdio.h>

int main(int argc, char **argv) {
    const VSAPI *vsapi = NULL;
    const VSSCRIPTAPI *vssapi = NULL;
    VSScript *se = NULL;
    FILE *outFile = NULL;

    if (argc != 3) {
        fprintf(stderr, "Usage: decmat <script> <matfile>\n");
        return 1;
    }

    // Open the output file for writing
    outFile = fopen(argv[2], "wb");

    if (!outFile) {
        fprintf(stderr, "Failed to open output for writing\n");
        return 1;
    }


    // Initialize VSScript and get the api pointer
    vssapi = getVSScriptAPI(VSSCRIPT_API_VERSION);
    if (!vssapi) {
        // VapourSynth probably isn't properly installed at all
        fprintf(stderr, "Failed to initialize VSScript library\n");
        return 1;
    }

    // Get a pointer to the normal api struct, exists so you don't have to link with the VapourSynth core library
    // Failure only happens on very rare API version mismatches and usually doesn't need to be checked
    vsapi = vssapi->getVSAPI(VAPOURSYNTH_API_VERSION);
    assert(vsapi);

    VSCore *core = vsapi->createCore(0);
    se = vssapi->createScript(core);

    // This line does the actual script evaluation. If se = NULL it will create a new environment
    vssapi->evaluateFile(se, argv[1]);   
    if (vssapi->getError(se)) {
        fprintf(stderr, "Script evaluation failed:\n%s", vssapi->getError(se));
        vssapi->freeScript(se);
        return 1;
    }

    // Get the clip set as output. It is valid until the out index is re-set/cleared/the script is freed
    VSNode *node = vssapi->getOutputNode(se, 0);
    if (!node) {
       fprintf(stderr, "Failed to retrieve output node\n");
       vssapi->freeScript(se);
       return 1;
    }

    // Reject hard to handle formats
    const VSVideoInfo *vi = vsapi->getVideoInfo(node);

    if (!vsh_isConstantVideoFormat(vi)) {
        fprintf(stderr, "Cannot output clips with varying dimensions or format\n");
        vsapi->freeNode(node);
        vssapi->freeScript(se);
        return 1;
    }

    // Output all frames
    char errMsg[1024];
    int error = 0;

    const VSFrame *frame = vsapi->getFrame(vi->numFrames - 1, node, errMsg, sizeof(errMsg)); //get last frame

    if (!frame) { // Check if an error happened when getting the frame
        error = 1;
        goto Cleanup;
    }

    // actually only grey plane in this demo
    int p = 0;
    ptrdiff_t stride = vsapi->getStride(frame, p);
    float *readPtr = (float*)vsapi->getReadPtr(frame, p);
    int cols = vsapi->getFrameWidth(frame, p);
    int rows = vsapi->getFrameHeight(frame, p);
    fprintf(outFile,"%d %d\n",rows,cols);
    // printf("%d %d\n",rows,cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(outFile,"%.10f ", readPtr[j]);
        }
        readPtr += stride/sizeof(float);
    }

    vsapi->freeFrame(frame);

Cleanup:
    fclose(outFile);

    vsapi->freeNode(node);
    vssapi->freeScript(se);

    if (error) {
        fprintf(stderr, "%s", errMsg);
        return 1;
    }

    return 0;
}