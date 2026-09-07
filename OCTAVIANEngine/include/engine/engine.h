#pragma once

void OCT_engine_init();
void OCT_engine_shutdown();
// void OCT_engine_start(char* name, unsigned int windowX, unsigned int windowY, OCT_vec4 BGColor, unsigned int virtualX, unsigned int virtualY, unsigned int maxFPS, unsigned int ECSHz, unsigned int physicsHz);
// void OCT_engine_stop();
void OCT_engine_frame_start();
void OCT_engine_frame_show();
void OCT_engine_updateContext(OCT_global context);