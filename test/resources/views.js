var render=(function(){'use strict';let BLANKS = [undefined, null, false];
function simpleLog(type, msg) {
	console.log(`[${type}] ${msg}`);
}
function awaitAll(total, callback) {
	let i = 0;
	return _ => {
		i++;
		if(i === total) {
			callback();
		}
	};
}
function flatCompact(items) {
	return items.reduce((memo, item) => {
		return BLANKS.indexOf(item) !== -1 ? memo :
				memo.concat(item.pop ? flatCompact(item) : item);
	}, []);
}
function blank(value) {
	return BLANKS.indexOf(value) !== -1;
}
function repr(value, jsonify = true) {
	return `\`${jsonify ? JSON.stringify(value) : value}\``;
}
function noop() {}let Fragment = {};
let VOID_ELEMENTS = {};
[
	"area", "base", "br", "col", "embed", "hr", "img", "input", "keygen",
	"link", "meta", "param", "source", "track", "wbr"
].forEach(tag => {
	VOID_ELEMENTS[tag] = true;
});
function generateHTML(tag, params, ...children) {
	return (stream, options, callback) => {
		let { nonBlocking, log = simpleLog, _idRegistry = {} } = options || {};
		if(tag !== Fragment) {
			let attribs = generateAttributes(params, { tag, log, _idRegistry });
			stream.write(`<${tag}${attribs}>`);
		}
		children = flatCompact(children);
		let isVoid = VOID_ELEMENTS[tag];
		let closingTag = (isVoid || tag === Fragment) ? null : tag;
		let total = children.length;
		if(total === 0) {
			closeElement(stream, closingTag, callback);
		} else {
			if(isVoid) {
				log("error", `void elements must not have children: \`<${tag}>\``);
			}
			let close = awaitAll(total, _ => {
				closeElement(stream, closingTag, callback);
			});
			processChildren(stream, children, 0,
					{ tag, nonBlocking, log, _idRegistry }, close);
		}
	};
}
function HTMLString(str) {
	if(blank(str) || !str.substr) {
		throw new Error(`invalid ${repr(this.constructor.name, false)}: ${repr(str)}`);
	}
	this.value = str;
}
function htmlEncode(str, attribute) {
	let res = str.replace(/&/g, "&amp;").
		replace(/</g, "&lt;").
		replace(/>/g, "&gt;");
	if(attribute) {
		res = res.replace(/"/g, "&quot;").
			replace(/'/g, "&#x27;");
	}
	return res;
}
function processChildren(stream, children, startIndex, options, callback) {
	for(let i = startIndex; i < children.length; i++) {
		let child = children[i];
		if(!child.call) {
			let content = child instanceof HTMLString ?
					child.value : htmlEncode(child.toString());
			stream.write(content);
			callback();
			continue;
		}
		let { nonBlocking, log, _idRegistry } = options;
		let generatorOptions = { nonBlocking, log, _idRegistry };
		if(child.length !== 1) {
			child(stream, generatorOptions, callback);
			continue;
		}
		let fn = element => {
			element(stream, generatorOptions, callback);
			let next = i + 1;
			if(next < children.length) {
				processChildren(stream, children, next, options, callback);
			}
		};
		if(!nonBlocking) {
			let invoked = false;
			let _fn = fn;
			fn = function() {
				invoked = true;
				return _fn.apply(null, arguments);
			};
			let _child = child;
			child = function() {
				let res = _child.apply(null, arguments);
				if(!invoked) {
					let msg = "invalid non-blocking operation detected";
					throw new Error(`${msg}: \`${options.tag}\``);
				}
				return res;
			};
		}
		child(fn);
		break;
	}
}
function closeElement(stream, tag, callback) {
	if(tag !== null) {
		stream.write(`</${tag}>`);
	}
	stream.flush();
	callback();
}
function generateAttributes(params, { tag, log, _idRegistry }) {
	if(!params) {
		return "";
	}
	if(_idRegistry && params.id !== undefined) {
		let { id } = params;
		if(_idRegistry[id]) {
			log("error", `duplicate HTML element ID: ${repr(params.id)}`);
		}
		_idRegistry[id] = true;
	}
	let attribs = Object.keys(params).reduce((memo, name) => {
		let value = params[name];
		switch(value) {
		case null:
		case undefined:
			break;
		case true:
			memo.push(name);
			break;
		case false:
			break;
		default:
			if(/ |"|'|>|'|\/|=/.test(name)) {
				reportAttribError(`invalid HTML attribute name: ${repr(name)}`, tag, log);
				break;
			}
			if(typeof value === "number") {
				value = value.toString();
			} else if(!value.substr) {
				reportAttribError(`invalid value for HTML attribute \`${name}\`: ` +
						`${repr(value)} (expected string)`, tag, log);
				break;
			}
			memo.push(`${name}="${htmlEncode(value, true)}"`);
		}
		return memo;
	}, []);
	return attribs.length === 0 ? "" : ` ${attribs.join(" ")}`;
}
function reportAttribError(msg, tag, log) {
	log("error", `${msg} - did you perhaps intend to use \`${tag}\` as a macro?`);
}function createElement(element, params, ...children) {
	if(element === undefined) {
		throw new Error("invalid macro: `undefined`");
	}
	return element.call ?
			element(params === null ? {} : params, ...flatCompact(children)) :
			generateHTML(element, params, ...children);
}
class Renderer {
	constructor({ doctype = "<!DOCTYPE html>", log } = {}) {
		this.doctype = doctype;
		this.log = log;
		this._macroRegistry = {};
		["registerView", "renderView"].forEach(meth => {
			this[meth] = this[meth].bind(this);
		});
	}
	registerView(macro, name = macro.name, replace) {
		if(!name) {
			throw new Error(`missing name for macro: \`${macro}\``);
		}
		let macros = this._macroRegistry;
		if(macros[name] && !replace) {
			throw new Error(`invalid macro name: \`${name}\` already registered`);
		}
		macros[name] = macro;
		return name;
	}
	renderView(view, params, stream, { fragment } = {}, callback) {
		if(!fragment) {
			stream.writeln(this.doctype);
		}
		if(fragment) {
			if(!params) {
				params = {};
			}
			params._layout = false;
		}
		let viewName = view && view.substr && view;
		let macro = viewName ? this._macroRegistry[viewName] : view;
		if(!macro) {
			throw new Error(`unknown view macro: \`${view}\` is not registered`);
		}
		let log = this.log && ((level, message) => this.log(level,
				`<${viewName || macro.name}> ${message}`));
		let element = createElement(macro, params);
		if(blank(element)) {
			element = createElement(Fragment);
		}
		if(callback) {
			element(stream, { nonBlocking: true, log }, callback);
		} else {
			element(stream, { nonBlocking: false, log }, noop);
		}
	}
}function Greeting(_ref) {
  var person = _ref.person;
  return createElement("html", null, createElement("head", null, createElement("meta", {
    charSet: "UTF-8"
  }), createElement("title", null, "Greeting | Example")), createElement("body", null, createElement("h1", null, "Hello ", person.name)));
}function Shell(_ref) {
  var _ref$title = _ref.title,
      title = _ref$title === void 0 ? 'complate-cpp-test' : _ref$title;
  for (var _len = arguments.length, children = new Array(_len > 1 ? _len - 1 : 0), _key = 1; _key < _len; _key++) {
    children[_key - 1] = arguments[_key];
  }
  return createElement("html", null, createElement("head", null, createElement("meta", {
    charSet: "UTF-8"
  }), createElement("meta", {
    name: "description",
    content: "JSX component for testing the library"
  }), createElement("title", null, title), createElement("link", {
    rel: "stylesheet",
    href: assets.link("css/bootstrap.min.css")
  })), createElement("body", null, children));
}function Layout(_ref) {
  var title = _ref.title;
  for (var _len = arguments.length, children = new Array(_len > 1 ? _len - 1 : 0), _key = 1; _key < _len; _key++) {
    children[_key - 1] = arguments[_key];
  }
  return createElement(Shell, {
    title: title
  }, createElement("main", {
    "class": "container"
  }, children));
}function TodoList(_ref) {
  var todos = _ref.todos;
  console.debug("some debug msg");
  console.log("some log msg");
  console.error("some error msg");
  return createElement(Layout, {
    title: "Render Test"
  }, createElement("h1", null, "TodoList"), createElement("small", {
    "class": "text-muted"
  }, "rendered by ", createElement("a", {
    href: getRendererLink(),
    target: "_blank"
  }, renderedBy)), createElement("div", {
    "class": "todolist"
  }, todos.map(TodoItem)));
}
function TodoItem(_ref2) {
  var what = _ref2.what,
      description = _ref2.description,
      updateLink = _ref2.updateLink,
      timespan = _ref2.timespan;
  return createElement("form", {
    "class": "mb-4",
    method: "GET",
    action: updateLink
  }, createElement("div", {
    "class": "card"
  }, createElement("div", {
    "class": "card-body"
  }, createElement("h5", {
    "class": "card-title"
  }, what), createElement("p", {
    "class": "card-text text-muted"
  }, description)), createElement("ul", {
    "class": "list-group"
  }, createElement(NeedDoBeDoneIn, {
    timespan: timespan
  })), createElement("div", {
    "class": "card-body"
  }, createElement("button", {
    type: "submit",
    value: "done",
    "class": "btn btn-sm btn-primary"
  }, "Done"))));
}
function NeedDoBeDoneIn(_ref3) {
  var timespan = _ref3.timespan;
  return createElement("li", {
    "class": "list-group-item is ".concat(timespan.veryLate && " bg-warning")
  }, createElement("dt", null, "Need do be done in"), createElement("dd", null, timespan.amount, " ", timespan.unit));
}var renderer = new Renderer({
  doctype: '<!DOCTYPE html>'
});
renderer.registerView(Greeting);
renderer.registerView(TodoList);
function render(view, params, stream) {
  renderer.renderView(view, params, stream);
}return render;}());