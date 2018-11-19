import re
import os
http_schema = re.compile(r"https?")
url_parser = re.compile(r"(\w+)://([\w\-@\.:]+)/?([\w/_\-@&\?\.=%()]+)?(#[\w\-@&_\?()/%]+)?")
base_dir = os.path.dirname(os.path.abspath(__file__))
sandbox_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "sandbox")
def parse_file(path):
    filename = os.path.join(sandbox_dir, path)
    if "./" in filename or ".." in filename:
        return "invalid content in url"
    if not filename.startswith(base_dir):
        return "url have to start with %s" % base_dir
    if filename.endswith("py") or "flag" in filename:
        return "invalid content in filename"
    if os.path.isdir(filename):
        file_list = os.listdir(filename)
        return ", ".join(file_list)
    elif os.path.isfile(filename):
        with open(filename, "rb") as f:
            content = f.read()
        return content
    else:
        return "can't find file"
def parse(url):
    fragments = url_parser.findall(url)
    if len(fragments) != 1 or len(fragments[0]) != 4:
        return("invalid url")
    schema = fragments[0][0]
    host = fragments[0][1]
    path = fragments[0][2]
    if http_schema.match(schema):
        return "It's a valid http url"
    elif schema == "file":
        if host != "sandbox":
            return "wrong file path"
        return parse_file(path)
    else:
        return "unknown schema"
