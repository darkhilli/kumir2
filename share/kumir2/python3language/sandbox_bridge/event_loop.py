import sys
import json
import importlib
import traceback
from sandbox_console import interpreter

cout = sys.__stdout__
cin = sys.__stdin__
cerr = sys.__stderr__

preloaded_modules = {}

globls = {}
interp = interpreter.Interpreter()

def do_pong():
    out_message = {
        "type": "pong"
    }
    message_line = json.dumps(out_message, separators=(',', ':'))
    cout.write(message_line)
    cout.write("\n")
    cout.flush()

def do_function_call(module_name, function_name, arguments):
    try:
        if module_name in preloaded_modules:
            module = preloaded_modules[module_name]
        else:
            module = importlib.import_module(module_name)
            preloaded_modules[module_name] = module
        function = module.__dict__[function_name]
        arguments_tuple = tuple(arguments)
        result = function(*arguments_tuple)
        out_message = {
            "type": "blocking_return",
            "return_value": result
        }
    except BaseException as e:
        error = repr(e)
        out_message = {
            "type": "exception",
            "string_data": error
        }
    message_line = json.dumps(out_message, separators=(',', ':'))
    cout.write(message_line)
    cout.write("\n")
    cout.flush()


def clean_traceback(tb):
    me = __file__
    frame = tb.tb_frame
    # internals = frame.__internals__
    code = frame.f_code
    filename = code.co_filename
    if me == filename:
        next = tb.tb_next
        return clean_traceback(next)
    else:
        return tb


def do_eval(async_id, eval_string):
    interp.runsource_separate_thread(async_id, eval_string)


def process():
    while True:
        message_line = str(cin.readline()).strip()
        if message_line:
            message = json.loads(message_line)
            cmd = message["type"].lower()
            if "exit" == cmd:
                cerr.write("Bye!")
                sys.exit(0)
            elif "ping" == cmd:
                do_pong()
            elif "blocking_call" == cmd:
                module_name = message["module_name"]
                function_name = message["function_name"]
                arguments = message["arguments"]
                do_function_call(module_name, function_name, arguments)
            elif "non_blocking_eval" == cmd:
                eval_string = message["eval_string"]
                async_id = message["async_id"]
                do_eval(async_id, eval_string)
            elif "input_response" == cmd:
                data = message["data"]
                sys.stdin.push(data)
            else:
                cerr.write("Unknown message type {}\n".format(cmd))
