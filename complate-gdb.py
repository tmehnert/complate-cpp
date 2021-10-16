# Copyright 2021 Torsten Mehnert
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
from string import digits
from subprocess import Popen, PIPE

def demangled_type(typename):
    name = str(typename).strip('"')
    try:
        type = gdb.lookup_type(name)
        return type
    except:
        pass

    name = name.lstrip(digits)
    try:
        type = gdb.lookup_type(name)
        return type
    except:
        pass

    process = Popen(["/usr/bin/c++filt", "-t", name], stdout=PIPE)
    (out, err) = process.communicate()
    name = out.decode("utf-8")
    try:
        type = gdb.lookup_type(name)
        return type
    except:
        pass

    return None

class ComplatePimplPrinter:
    def __init__(self, val, typename):
        self.val = val
        self.m_impl = val['m_impl']['_M_t'].cast(gdb.lookup_type(f"{typename}*")).dereference()

class ComplatePropertyPrinter:
    def __init__(self, val):
        ComplatePimplPrinter.__init__(self, val, 'complate::Property::Impl')

    def to_string(self):
        has_setter = self.m_impl['m_setter']['_M_payload']['_M_engaged']
        return "%s, %s" % (self.m_impl['m_name'], 'writable' if has_setter else 'readonly')


class ComplateMethodPrinter:
    def __init__(self, val):
        ComplatePimplPrinter.__init__(self, val, 'complate::Method::Impl')

    def to_string(self):
        return "%s" % self.m_impl['m_name']

class ComplatePrototypePrinter:
    def __init__(self, val):
        ComplatePimplPrinter.__init__(self, val, 'complate::Prototype::Impl')

    def children(self):
        yield "name", self.m_impl['m_name']
        yield "methods", self.m_impl['m_methods']
        yield "properties", self.m_impl['m_properties']


class ComplateProxyPrinter:
    def __init__(self, val):
        self.val = val

    def children(self):
        name = self.val['m_name']
        object = self.val['m_object']
        type = demangled_type(name)
        if type is not None:
            #type = gdb.lookup_type("std::shared_ptr<%s>" % type)
            object = object.cast(type.pointer())
        yield "name", name
        yield "object", object


class ComplateProxyWeakPrinter:
    def __init__(self, val):
        self.val = val

    def children(self):
        name = self.val['m_name']
        object = self.val['m_object']
        type = demangled_type(name)
        if type is not None:
            object = object.cast(type.pointer())
        yield "name", name
        yield "object", object

class ComplateVariantPrinter:
    def __init__(self, val):
        self.val = val
        self.m_data = self.val['m_data']
        self.index = self.val['m_data']['_M_index']

    def cast(self, typedict):
        typename = typedict.get(int(self.index))
        if typename is None:
            return None
        return self.m_data.cast(gdb.lookup_type(typename))

    def format_string(self, typedict):
        typename = typedict.get(int(self.index))
        if typename is None:
            return None
        return "{%s} %s" % (typename, self.m_data.cast(gdb.lookup_type(typename)))


class ComplateStringPrinter(ComplateVariantPrinter):
    def __init__(self, val):
        ComplateVariantPrinter.__init__(self, val)

    def to_string(self):
        return self.format_string({
            0: 'std::string',
            1: 'std::string_view'
        })


class ComplateNumberPrinter(ComplateVariantPrinter):
    def __init__(self, val):
        ComplateVariantPrinter.__init__(self, val)

    def to_string(self):
        return self.format_string({
            0: 'int32_t',
            1: 'uint32_t',
            2: 'int64_t',
            3: 'double'
        })

class ComplateValuePrinter(ComplateVariantPrinter):
    def __init__(self, val):
        ComplateVariantPrinter.__init__(self, val)

    def children(self):
        yield "[contained value]", self.cast({
            0: 'std::monostate',
            1: 'nullptr_t',
            2: 'bool',
            3: 'complate::Number',
            4: 'complate::String',
            5: 'complate::Array',
            6: 'complate::Object',
            7: 'complate::Function',
            8: 'complate::Proxy',
            9: 'complate::ProxyWeak'
        })



def build_printer():
    printer = gdb.printing.RegexpCollectionPrettyPrinter("complate")
    printer.add_printer('complate::Prototype', '^complate::Prototype$', ComplatePrototypePrinter)
    printer.add_printer('complate::Property', '^complate::Property$', ComplatePropertyPrinter)
    printer.add_printer('complate::Method', '^complate::Method$', ComplateMethodPrinter)
    printer.add_printer('complate::Value', '^complate::Value$', ComplateValuePrinter)
    printer.add_printer('complate::String', '^complate::String$', ComplateStringPrinter)
    printer.add_printer('complate::Number', '^complate::Number$', ComplateNumberPrinter)
    printer.add_printer('complate::Proxy', '^complate::Proxy$', ComplateProxyPrinter)
    printer.add_printer('complate::ProxyWeak', '^complate::ProxyWeak$', ComplateProxyWeakPrinter)
    return printer

gdb.printing.register_pretty_printer(gdb.current_objfile(), build_printer(), replace=True)