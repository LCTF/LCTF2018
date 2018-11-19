from flask import Flask, escape, request, make_response, render_template

from session import *
from utils import *
from flag import FLAG
from parser import parse

app = Flask(__name__)

import logging
'''
from logging.handlers import HTTPHandler
http_handler = HTTPHandler(
    host="127.0.0.1:6666",
    url="log",
)
http_handler.setLevel(logging.ERROR)
http_handler.setFormatter(logging.Formatter(
    '[%(asctime)s] %(levelname)s in %(module)s: %(message)s'
))
app.logger.addHandler(http_handler)
'''
file_handler = logging.FileHandler('flask.log', encoding='UTF-8')
file_handler.setLevel(logging.INFO)
logging_format = logging.Formatter(
    '%(asctime)s - %(levelname)s - %(filename)s - %(funcName)s - %(lineno)s - %(message)s')
file_handler.setFormatter(logging_format)
app.logger.addHandler(file_handler)

@app.route('/')
def index():
    user = request.cookies.get("user", "")
    try:
        username = session_decode(user)
    except Exception:
        username = get_username()
        resp = make_response(render_template('main.html', content=escape(username)))
        resp.set_cookie("user", session_encode(username),
                        expires=datetime.datetime.today() + datetime.timedelta(hours=3))
    else:
        if username == "admin":
            app.logger.error(log_ip(request.remote_addr, "get flag"))
            content=escape(FLAG)
        else:
            content=escape(username)
        resp = make_response(render_template('main.html', content=content))
    return resp

@app.route('/sandbox')
def render_static():
    if not check_token(request.args.get("token")):
        resp = make_response("invalid request")
    else:
        url = request.args.get("url")
        try:
            if url is None or url == "":
                content = "no url input"
            else:
                app.logger.info(log_ip(request.remote_addr, url))
                content = parse(url)
            resp = make_response(content)
        except Exception:
            resp = make_response("url error")
    resp.mimetype = "text/plain"
    return resp

if __name__ == '__main__':
    app.run()

