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
#include <complate/quickjs/quickjsrendererbuilder.h>
#include <complate/core/reevaluatingrenderer.h>

#include <fstream>
#include <sstream>

#include "httplib.h"

using namespace std;
using namespace complate;

string getViews() {
  ifstream ifs = ifstream(TEST_RESOURCE_DIR + string("views.js"));
  stringstream ss;
  ss << ifs.rdbuf();
  return ss.str();
}

int main() {
  /* During Development a ReEvaluating Renderer is nice, because you
   * can edit the JSX templates and don't need to restart your application.
   * Don't do this for your Releases, because the source will be evaluated
   * each time you render page.
   *
   * Just run "npm start" in a terminal, and then edit (views/greeting.jsx)
   * to try out.
   */
  // clang-format-off
  auto renderer = ReEvaluatingRenderer(QuickJsRendererBuilder()
      .source(getViews)
      .creator()
  );
  // clang-format on

  httplib::Server server;
  server.Get("/", [&](auto &, auto &res) {
    /* Define some data passed to the view */
    Object parameters;
    parameters.emplace("person", Object{{"name", "John Doe"}});

    /* Render Greeting, found at (views/greeting.jsx) */
    res.body = renderer.renderToString("Greeting", parameters);
    res.status = 200;
    res.set_header("Content-Type", "text/html");
  });
  server.set_exception_handler([](auto &, auto &res, exception &e) {
    /* Just to give you a hint, where you can solve the problem */
    stringstream ss;
    ss << "<html>"
       << "<head><title>Oh no!</title><head>"
       << "<body>"
       << "<h1>It looks like a syntax error in jsx</h1>"
       << "<h2>Fix it and reload your browser</h2>"
       << "<p>Your 'npm start' output may help you.</p>"
       << "<pre>" << e.what() << "</pre>"
       << "</body>"
       << "</html>";
    res.body = ss.str();
    res.status = 500;
    res.set_header("Content-Type", "text/html");
  });

  cout << "Starting server at http://localhost:8080" << endl;
  server.listen("127.0.0.1", 8080);
  return EXIT_SUCCESS;
}
