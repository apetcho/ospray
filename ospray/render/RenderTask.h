// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

// ospcommon
#include "ospcommon/tasking/AsyncTask.h"
// ospray
#include "common/QueryableTask.h"
#include "fb/FrameBuffer.h"

namespace ospray {

  struct RenderTask : public QueryableTask, public tasking::AsyncTask<float>
  {
    RenderTask(FrameBuffer *, std::function<float()> fcn);
    ~RenderTask() override = default;

    bool isFinished() override;
    void wait(OSPSyncEvent event) override;
    void cancel() override;
    float getProgress() override;

   private:
    Ref<FrameBuffer> fb;
  };

  // Inlined definitions //////////////////////////////////////////////////////

  inline RenderTask::RenderTask(FrameBuffer *_fb, std::function<float()> fcn)
      : AsyncTask<float>(fcn), fb(_fb)
  {
  }

  inline bool RenderTask::isFinished()
  {
    return finished();
  }

  inline void RenderTask::wait(OSPSyncEvent event)
  {
    if (event == OSP_FRAME_FINISHED)
      AsyncTask<float>::wait();
    else
      fb->waitForEvent(event);
  }

  inline void RenderTask::cancel()
  {
    fb->cancelFrame();
  }

  inline float RenderTask::getProgress()
  {
    return fb->getCurrentProgress();
  }

}  // namespace ospray
