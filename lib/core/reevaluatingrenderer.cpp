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
#include <complate/core/reevaluatingrenderer.h>

using namespace complate;
using namespace std;

class ReEvaluatingRenderer::Impl {
public:
  explicit Impl(Creator creator) : m_creator(move(creator)) {}

  void render(const string &view, const Object &parameters, Stream &stream) {
    m_creator()->render(view, parameters, stream);
  }

  void render(const string &view, const string &parameters, Stream &stream) {
    m_creator()->render(view, parameters, stream);
  }

private:
  Creator m_creator;
};

ReEvaluatingRenderer::ReEvaluatingRenderer(Creator creator)
    : m_impl(make_unique<Impl>(move(creator))) {}
ReEvaluatingRenderer::~ReEvaluatingRenderer() = default;

void ReEvaluatingRenderer::render(const string &view, const Object &parameters,
                                  Stream &stream) {
  m_impl->render(view, parameters, stream);
}

void ReEvaluatingRenderer::render(const string &view, const string &parameters,
                                  Stream &stream) {
  m_impl->render(view, parameters, stream);
}
