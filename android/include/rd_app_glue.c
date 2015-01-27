/*
 * Copyright (C) 2010 The Android Open Source Project
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
 *
 */

#include <jni.h>

#import <errno.h>
#import <string.h>
#import <unistd.h>
#import <sys/resource.h>
#import <EGL/egl.h>
#import <GLES/gl.h>
#import "rd_app_glue.h"
#import <android/log.h>

#ifndef RD_LOG
#define RD_LOG
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "threaded_app", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "threaded_app", __VA_ARGS__))
#endif

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

//ANativeWindow *_nativeWindow = NULL;

#pragma mark - Static functions

//static Display *sDisplay;
//static Window sWindow;
static int sWindowWidth = 480;//WINDOW_DEFAULT_WIDTH;
static int sWindowHeight = 800;//WINDOW_DEFAULT_HEIGHT;
static EGLDisplay sEglDisplay = EGL_NO_DISPLAY;
static EGLConfig sEglConfig;
static EGLContext sEglContext = EGL_NO_CONTEXT;
static EGLSurface sEglSurface = EGL_NO_SURFACE;

static void printConfig(EGLConfig config)
{
    EGLint value;
    
    LOGV("EGL configuration: %p", config);
    eglGetConfigAttrib(sEglDisplay, config, EGL_RED_SIZE, &value);
    LOGV("  RED_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_GREEN_SIZE, &value);
    LOGV("  GREEN_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_BLUE_SIZE, &value);
    LOGV("  BLUE_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_ALPHA_SIZE, &value);
    LOGV("  ALPHA_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_DEPTH_SIZE, &value);
    LOGV("  DEPTH_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_STENCIL_SIZE, &value);
    LOGV("  STENCIL_SIZE = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_SAMPLE_BUFFERS, &value);
    LOGV("  SAMPLE_BUFFERS = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_SAMPLES, &value);
    LOGV("  SAMPLES = %d", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_SURFACE_TYPE, &value);
    LOGV("  SURFACE_TYPE = %p", value);
    eglGetConfigAttrib(sEglDisplay, config, EGL_CONFIG_CAVEAT, &value);
    LOGV("  CONFIG_CAVEAT = %p", value);
}

static int initGraphics()
{
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    /*
     static const EGLint configAttribs[] =
     {
     EGL_RED_SIZE,       8,
     EGL_GREEN_SIZE,     8,
     EGL_BLUE_SIZE,      8,
     EGL_DEPTH_SIZE,     16,
     EGL_ALPHA_SIZE,     EGL_DONT_CARE,
     EGL_STENCIL_SIZE,   EGL_DONT_CARE,
     EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
     EGL_NONE
     };*/
    EGLBoolean success;
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    
    //int importGLResult;
    //importGLResult = importGLInit();
    //if (!importGLResult)
    //    return 0;
    
    //sDisplay = XOpenDisplay(NULL);
    
    sEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    //LOGV("eglGetDisplay sEglDisplay: %p", sEglDisplay);
    success = eglInitialize(sEglDisplay, &majorVersion, &minorVersion);
    //LOGV("eglInitialize success: %d", success);
    //if (success != EGL_FALSE)
    //    success = eglGetConfigs(sEglDisplay, NULL, 0, &numConfigs);
    //if (success != EGL_FALSE) {
    success = eglChooseConfig(sEglDisplay, attribs, &sEglConfig, 1, &numConfigs);
    //LOGV("eglChooseConfig success: %d", success);
    
    //printConfig(sEglConfig);
    //}
    //if (success != EGL_FALSE) {
    
    //if (success != EGL_FALSE)
    //{
    //XSetWindowAttributes swa;
    //XVisualInfo *vi, tmp;
    //XSizeHints sh;
    int n;
    EGLint format;
    
    eglGetConfigAttrib(sEglDisplay, sEglConfig, EGL_NATIVE_VISUAL_ID, &format);
    LOGV("eglGetConfigAttrib format: %d", format);
    //success = ANativeWindow_setBuffersGeometry(_nativeWindow, 0, 0, format);
    LOGV("ANativeWindow_setBuffersGeometry success: %d", success);
    //tmp.visualid = vid;
    //vi = XGetVisualInfo(sDisplay, VisualIDMask, &tmp, &n);
    /* swa.colormap = XCreateColormap(sDisplay,
     RootWindow(sDisplay, vi->screen),
     vi->visual, AllocNone);
     sh.flags = PMinSize | PMaxSize;
     sh.min_width = sh.max_width = sWindowWidth;
     sh.min_height = sh.max_height = sWindowHeight;
     swa.border_pixel = 0;
     swa.event_mask = ExposureMask | StructureNotifyMask |
     KeyPressMask | ButtonPressMask | ButtonReleaseMask;
     sWindow = XCreateWindow(sDisplay, RootWindow(sDisplay, vi->screen),
     0, 0, sWindowWidth, sWindowHeight,
     0, vi->depth, InputOutput, vi->visual,
     CWBorderPixel | CWColormap | CWEventMask,
     &swa);
     XMapWindow(sDisplay, sWindow);
     XSetStandardProperties(sDisplay, sWindow, sAppName, sAppName,
     None, (void *)0, 0, &sh);*/
    //}
    //if (success != EGL_FALSE) {
    //sEglSurface = eglCreateWindowSurface(sEglDisplay, sEglConfig, _nativeWindow, NULL);
    //LOGV("eglCreateWindowSurface sEglSurface: %d", sEglSurface);
    //LOGV("eglCreateWindowSurface EGL_NO_SURFACE: %d", EGL_NO_SURFACE);
    if (sEglSurface == EGL_NO_SURFACE) {
        LOGV("sEglSurface == EGL_NO_SURFACE");
        success = EGL_FALSE;
    } else {
        success = EGL_TRUE;
    }
    //}
    LOGV("eglCreateWindowSurface success: %d", success);
    sEglContext = eglCreateContext(sEglDisplay, sEglConfig, NULL, NULL);
    if (sEglContext == EGL_NO_CONTEXT) {
        success = EGL_FALSE;
    } else {
        success = EGL_TRUE;
    }
    //}
    LOGV("eglCreateContext success: %d", success);
    //if (success != EGL_FALSE) {
    success = eglMakeCurrent(sEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    LOGV("eglMakeCurrent EGL_NO_SURFACE success: %d", success);
    success = eglMakeCurrent(sEglDisplay, sEglSurface, sEglSurface, sEglContext);
    //}
    LOGV("eglMakeCurrent success: %d", success);
    //if (success == EGL_FALSE) {
    //LOGE("eglGetError: %d", eglGetError());
    //checkEGLErrors();
    //}
    //LOGV("4 success: %d", success);
    //eglSwapBuffers(sEglDisplay, sEglSurface);
    return success != EGL_FALSE;
}

static void _RDSetup()
{
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    //glEnable(GL_CULL_FACE);
    //glShadeModel(GL_SMOOTH);
    //glDisable(GL_DEPTH_TEST);
    
    LOGV("_RDSetup");
/*    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_ALPHA_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
}

static void _RDClear()
{
    LOGV("_RDClear");
    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void _RDSwapBuffers()
{
    LOGV("_RDSwapBuffers");
    //EAGLContext *currentContext = _EAGLGetCurrentContext();
    //DLog(@"currentContext->_eglDisplay: %p, currentContext->_eglSurface: %p", currentContext->_eglDisplay, currentContext->_eglSurface);
    eglSwapBuffers(sEglDisplay, sEglSurface);
    //DLog();
    //_EAGLSwappingBuffers = NO;
}

static void free_saved_state(struct android_app* android_app) {
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->savedState != NULL) {
        free(android_app->savedState);
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }
    pthread_mutex_unlock(&android_app->mutex);
}

int8_t android_app_read_cmd(struct android_app* android_app) {
    int8_t cmd;
    if (read(android_app->msgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
        switch (cmd) {
            case APP_CMD_SAVE_STATE:
                free_saved_state(android_app);
                break;
        }
        return cmd;
    } else {
        LOGE("No data on command pipe!");
    }
    return -1;
}

static void print_cur_config(struct android_app* android_app) {
    char lang[2], country[2];
    AConfiguration_getLanguage(android_app->config, lang);
    AConfiguration_getCountry(android_app->config, country);

    LOGV("Config: mcc=%d mnc=%d lang=%c%c cnt=%c%c orien=%d touch=%d dens=%d "
            "keys=%d nav=%d keysHid=%d navHid=%d sdk=%d size=%d long=%d "
            "modetype=%d modenight=%d",
            AConfiguration_getMcc(android_app->config),
            AConfiguration_getMnc(android_app->config),
            lang[0], lang[1], country[0], country[1],
            AConfiguration_getOrientation(android_app->config),
            AConfiguration_getTouchscreen(android_app->config),
            AConfiguration_getDensity(android_app->config),
            AConfiguration_getKeyboard(android_app->config),
            AConfiguration_getNavigation(android_app->config),
            AConfiguration_getKeysHidden(android_app->config),
            AConfiguration_getNavHidden(android_app->config),
            AConfiguration_getSdkVersion(android_app->config),
            AConfiguration_getScreenSize(android_app->config),
            AConfiguration_getScreenLong(android_app->config),
            AConfiguration_getUiModeType(android_app->config),
            AConfiguration_getUiModeNight(android_app->config));
}

void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_INPUT_CHANGED:
            LOGV("APP_CMD_INPUT_CHANGED\n");
            pthread_mutex_lock(&android_app->mutex);
            if (android_app->inputQueue != NULL) {
                AInputQueue_detachLooper(android_app->inputQueue);
            }
            android_app->inputQueue = android_app->pendingInputQueue;
            if (android_app->inputQueue != NULL) {
                LOGV("Attaching input queue to looper");
                AInputQueue_attachLooper(android_app->inputQueue,
                        android_app->looper, LOOPER_ID_INPUT, NULL,
                        &android_app->inputPollSource);
            }
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_INIT_WINDOW:
            LOGV("APP_CMD_INIT_WINDOW\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = android_app->pendingWindow;
            /*_nativeWindow = android_app->pendingWindow;
            //_RDCreateContext();
            initGraphics();
            _RDSetup();
            _RDClear();
            _RDSwapBuffers();*/
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            pthread_cond_broadcast(&android_app->cond);
            break;

        case APP_CMD_RESUME:
        case APP_CMD_START:
        case APP_CMD_PAUSE:
        case APP_CMD_STOP:
            //LOGV("activityState=%d\n", cmd);
            pthread_mutex_lock(&android_app->mutex);
            android_app->activityState = cmd;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGV("APP_CMD_CONFIG_CHANGED\n");
            AConfiguration_fromAssetManager(android_app->config,
                    android_app->activity->assetManager);
            print_cur_config(android_app);
            break;

        case APP_CMD_DESTROY:
            LOGV("APP_CMD_DESTROY\n");
            android_app->destroyRequested = 1;
            break;
    }
}

void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd) {
    switch (cmd) {
        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->window = NULL;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_SAVE_STATE:
            LOGV("APP_CMD_SAVE_STATE\n");
            pthread_mutex_lock(&android_app->mutex);
            android_app->stateSaved = 1;
            pthread_cond_broadcast(&android_app->cond);
            pthread_mutex_unlock(&android_app->mutex);
            break;

        case APP_CMD_RESUME:
            free_saved_state(android_app);
            break;
    }
}

void app_dummy() {

}

static void android_app_destroy(struct android_app* android_app) {
    LOGV("android_app_destroy!");
    free_saved_state(android_app);
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->inputQueue != NULL) {
        AInputQueue_detachLooper(android_app->inputQueue);
    }
    AConfiguration_delete(android_app->config);
    android_app->destroyed = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);
    // Can't touch android_app object after this.
}

static void process_input(struct android_app* app, struct android_poll_source* source) {
    AInputEvent* event = NULL;
    while (AInputQueue_getEvent(app->inputQueue, &event) >= 0) {
        //LOGV("New input event: type=%d\n", AInputEvent_getType(event));
        if (AInputQueue_preDispatchEvent(app->inputQueue, event)) {
            continue;
        }
        int32_t handled = 0;
        if (app->onInputEvent != NULL) handled = app->onInputEvent(app, event);
        AInputQueue_finishEvent(app->inputQueue, event, handled);
    }
}

static void process_cmd(struct android_app* app, struct android_poll_source* source) {
    //LOGV("process_cmd 1");
    int8_t cmd = android_app_read_cmd(app);
    //LOGV("process_cmd cmd: %d", cmd);
    //LOGV("process_cmd android_app_pre_exec_cmd cmd: %d", cmd);
    android_app_pre_exec_cmd(app, cmd);
    if (app->onAppCmd != NULL) {
        app->onAppCmd(app, cmd);
    }
    android_app_post_exec_cmd(app, cmd);
}

static void* android_app_entry(void* param) {
    struct android_app* android_app = (struct android_app*)param;

    android_app->config = AConfiguration_new();
    AConfiguration_fromAssetManager(android_app->config, android_app->activity->assetManager);

    print_cur_config(android_app);

    android_app->cmdPollSource.id = LOOPER_ID_MAIN;
    android_app->cmdPollSource.app = android_app;
    android_app->cmdPollSource.process = process_cmd;
    android_app->inputPollSource.id = LOOPER_ID_INPUT;
    android_app->inputPollSource.app = android_app;
    android_app->inputPollSource.process = process_input;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    ALooper_addFd(looper, android_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
            &android_app->cmdPollSource);
    android_app->looper = looper;

    pthread_mutex_lock(&android_app->mutex);
    android_app->running = 1;
    pthread_cond_broadcast(&android_app->cond);
    pthread_mutex_unlock(&android_app->mutex);

    android_main(android_app);

    android_app_destroy(android_app);
    return NULL;
}

// --------------------------------------------------------------------
// Native activity interaction (called from main thread)
// --------------------------------------------------------------------

static struct android_app* android_app_create(ANativeActivity* activity,
        void* savedState, size_t savedStateSize) {
    struct android_app* android_app = (struct android_app*)malloc(sizeof(struct android_app));
    memset(android_app, 0, sizeof(struct android_app));
    android_app->activity = activity;

    pthread_mutex_init(&android_app->mutex, NULL);
    pthread_cond_init(&android_app->cond, NULL);

    if (savedState != NULL) {
        android_app->savedState = malloc(savedStateSize);
        android_app->savedStateSize = savedStateSize;
        memcpy(android_app->savedState, savedState, savedStateSize);
    }

    int msgpipe[2];
    if (pipe(msgpipe)) {
        LOGE("could not create pipe: %s", strerror(errno));
        return NULL;
    }
    android_app->msgread = msgpipe[0];
    android_app->msgwrite = msgpipe[1];

    pthread_attr_t attr; 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&android_app->thread, &attr, android_app_entry, android_app);

    // Wait for thread to start.
    pthread_mutex_lock(&android_app->mutex);
    while (!android_app->running) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);

    return android_app;
}

static void android_app_write_cmd(struct android_app* android_app, int8_t cmd)
{
    if (write(android_app->msgwrite, &cmd, sizeof(cmd)) != sizeof(cmd)) {
        LOGE("Failure writing android_app cmd: %s\n", strerror(errno));
    }
}

static void android_app_set_input(struct android_app* android_app, AInputQueue* inputQueue)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app->pendingInputQueue = inputQueue;
    android_app_write_cmd(android_app, APP_CMD_INPUT_CHANGED);
    while (android_app->inputQueue != android_app->pendingInputQueue) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_window(struct android_app* android_app, ANativeWindow* window)
{
    pthread_mutex_lock(&android_app->mutex);
    if (android_app->pendingWindow != NULL) {
        android_app_write_cmd(android_app, APP_CMD_TERM_WINDOW);
    }
    android_app->pendingWindow = window;
    
    //android_app->window2 = android_app->pendingWindow;
    //_nativeWindow = android_app->pendingWindow;
    //_RDCreateContext();
    //initGraphics();
    //_RDSetup();
    //_RDClear();
    //_RDSwapBuffers();
    
    if (window != NULL) {
        android_app_write_cmd(android_app, APP_CMD_INIT_WINDOW);
    }
    while (android_app->window != android_app->pendingWindow) {
        LOGV("pthread_cond_wait");
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_set_activity_state(struct android_app* android_app, int8_t cmd)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, cmd);
    while (android_app->activityState != cmd) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);
}

static void android_app_free(struct android_app* android_app)
{
    pthread_mutex_lock(&android_app->mutex);
    android_app_write_cmd(android_app, APP_CMD_DESTROY);
    while (!android_app->destroyed) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }
    pthread_mutex_unlock(&android_app->mutex);

    close(android_app->msgread);
    close(android_app->msgwrite);
    pthread_cond_destroy(&android_app->cond);
    pthread_mutex_destroy(&android_app->mutex);
    free(android_app);
}

static void onDestroy(ANativeActivity* activity)
{
    LOGV("Destroy: %p\n", activity);
    android_app_free((struct android_app*)activity->instance);
}

static void onStart(ANativeActivity* activity)
{
    LOGV("Start: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_START);
}

static void onResume(ANativeActivity* activity)
{
    LOGV("Resume: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_RESUME);
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    void* savedState = NULL;

    LOGV("SaveInstanceState: %p\n", activity);
    pthread_mutex_lock(&android_app->mutex);
    android_app->stateSaved = 0;
    android_app_write_cmd(android_app, APP_CMD_SAVE_STATE);
    while (!android_app->stateSaved) {
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }

    if (android_app->savedState != NULL) {
        savedState = android_app->savedState;
        *outLen = android_app->savedStateSize;
        android_app->savedState = NULL;
        android_app->savedStateSize = 0;
    }

    pthread_mutex_unlock(&android_app->mutex);

    return savedState;
}

static void onPause(ANativeActivity* activity)
{
    LOGV("Pause: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_PAUSE);
}

static void onStop(ANativeActivity* activity)
{
    LOGV("Stop: %p\n", activity);
    android_app_set_activity_state((struct android_app*)activity->instance, APP_CMD_STOP);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    LOGV("ConfigurationChanged: %p\n", activity);
    android_app_write_cmd(android_app, APP_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity* activity)
{
    struct android_app* android_app = (struct android_app*)activity->instance;
    LOGV("LowMemory: %p\n", activity);
    android_app_write_cmd(android_app, APP_CMD_LOW_MEMORY);
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    LOGV("WindowFocusChanged: %p -- %d\n", activity, focused);
    struct android_app* android_app = activity->instance;
    /*while (1) {
        LOGV("pthread_cond_wait");
        pthread_cond_wait(&android_app->cond, &android_app->mutex);
    }*/
    android_app_write_cmd((struct android_app*)activity->instance,
                          focused ? APP_CMD_GAINED_FOCUS : APP_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    LOGV("NativeWindowCreated: %p -- %p\n", activity, window);
    android_app_set_window((struct android_app*)activity->instance, window);
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    LOGV("NativeWindowDestroyed: %p -- %p\n", activity, window);
    android_app_set_window((struct android_app*)activity->instance, NULL);
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    //LOGV("InputQueueCreated: %p -- %p\n", activity, queue);
    android_app_set_input((struct android_app*)activity->instance, queue);
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    LOGV("InputQueueDestroyed: %p -- %p\n", activity, queue);
    android_app_set_input((struct android_app*)activity->instance, NULL);
}

void ANativeActivity_onCreate(ANativeActivity* activity,
        void* savedState, size_t savedStateSize)
{
    LOGV("Creating: %p\n", activity);
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity->instance = android_app_create(activity, savedState, savedStateSize);
}
