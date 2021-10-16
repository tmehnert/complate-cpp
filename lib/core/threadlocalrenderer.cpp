/**
 * Copyright 2021 Torsten Mehnert
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#include <complate/core/threadlocalrenderer.h>

using namespace complate;
using namespace std;

thread_local unique_ptr<Renderer> COMPLATE_THREAD_LOCAL_RENDERER_INSTANCE =
    nullptr;

class ThreadLocalRenderer::Impl {
public:
  explicit Impl(Creator creator) : m_creator(move(creator)) {}

  void render(const string &view, const Object &parameters, Stream &stream) {
    getOrCreateRenderer()->render(view, parameters, stream);
  }

  void render(const string &view, const string &parameters, Stream &stream) {
    getOrCreateRenderer()->render(view, parameters, stream);
  }

  static void reset() {
    COMPLATE_THREAD_LOCAL_RENDERER_INSTANCE.reset(nullptr);
  }

  unique_ptr<Renderer> &getOrCreateRenderer() {
    if (!COMPLATE_THREAD_LOCAL_RENDERER_INSTANCE) {
      COMPLATE_THREAD_LOCAL_RENDERER_INSTANCE = m_creator();
    }

    return COMPLATE_THREAD_LOCAL_RENDERER_INSTANCE;
  }

private:
  Creator m_creator;
};

ThreadLocalRenderer::ThreadLocalRenderer(Creator creator)
    : m_impl(make_unique<Impl>(move(creator))) {}
ThreadLocalRenderer::~ThreadLocalRenderer() = default;

void ThreadLocalRenderer::render(const string &view, const Object &parameters,
                                 Stream &stream) {
  m_impl->render(view, parameters, stream);
}

void ThreadLocalRenderer::render(const string &view, const string &parameters,
                                 Stream &stream) {
  m_impl->render(view, parameters, stream);
}

void ThreadLocalRenderer::reset() { m_impl->reset(); }
