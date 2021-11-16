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
/* global renderedBy, getRendererLink */
import {createElement} from 'complate-stream';
import Layout from './components/layout';

export default function TodoList({todos}) {
    // console will not print anything. Add an binding 'console' to use.
    console.debug("some debug msg")
    console.log("some log msg")
    console.error("some error msg")

    return <Layout title="Render Test">
        <h1>TodoList</h1>
        <small class="text-muted">rendered by <a href={getRendererLink()} target="_blank">{renderedBy}</a></small>
        <div class="todolist">
            {todos.map(TodoItem)}
        </div>
    </Layout>
}

function TodoItem({what, description, needToBeDoneIn, veryLate, updateLink}) {
    return <form class="mb-4" method="GET" action={updateLink}>
        <div class="card">
            <div class="card-body">
                <h5 class="card-title">{what}</h5>
                <p class="card-text text-muted">{description}</p>
            </div>
            <ul class="list-group">
                <NeedDoBeDoneIn needToBeDoneIn={needToBeDoneIn} veryLate={veryLate}/>
            </ul>
            <div class="card-body">
                <button type="submit" value="done" class="btn btn-sm btn-primary">Done</button>
            </div>
        </div>
    </form>
}

function NeedDoBeDoneIn({needToBeDoneIn, veryLate}) {
    return <li class={`list-group-item is ${veryLate && " bg-warning"}`}>
        <dt>Need do be done in</dt>
        <dd>{needToBeDoneIn}</dd>
    </li>
}
