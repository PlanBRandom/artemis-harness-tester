from flask import Flask, render_template_string, request
import csv

app = Flask(__name__)

TEMPLATE = """
<!doctype html>
<title>Artemis Harness Matrix</title>
<h1>Harness Matrix</h1>
<form method="post" enctype="multipart/form-data">
  <input type="file" name="file">
  <input type="submit" value="Upload">
</form>
{% if header %}
<table border="1" cellpadding="4" cellspacing="0">
  <tr>
    {% for h in header %}
      <th>{{ h }}</th>
    {% endfor %}
  </tr>
  {% for row in rows %}
    <tr>
      {% for cell in row %}
        <td>{{ cell }}</td>
      {% endfor %}
    </tr>
  {% endfor %}
</table>
{% endif %}
"""

@app.route("/", methods=["GET", "POST"])
def index():
    header = None
    rows = []
    if request.method == "POST":
        file = request.files.get("file")
        if file:
            reader = csv.reader(file.stream.read().decode().splitlines())
            header = next(reader, None)
            rows = list(reader)
    return render_template_string(TEMPLATE, header=header, rows=rows)

if __name__ == "__main__":
    app.run(debug=True)
