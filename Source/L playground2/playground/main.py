from flask import Flask, escape, request, make_response, render_template

from session import *
from utils import *
from flag import FLAG
from parser import parse

app = Flask(__name__)

@app.route('/')
def index():
    user = request.cookies.get("user", "")
    try:
        username = session_decode(user)
    except Exception:
        username = get_username()
        content=escape(username)
    else:
        if username == "admin":
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
                content = parse(url)
            resp = make_response(content)
        except Exception:
            resp = make_response("url error")
    resp.mimetype = "text/plain"
    return resp

app.run(port=5000)