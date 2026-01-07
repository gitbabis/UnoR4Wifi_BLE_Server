#ifndef WEB_ASSETS_H
#define WEB_ASSETS_H

const char css_content[] PROGMEM = R"rawliteral(
body { font-family: sans-serif; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; background-color: #f0f2f5; margin: 0; }
h1 { color: #333; margin-bottom: 20px; }
.switch { position: relative; display: inline-block; width: 60px; height: 34px; }
.switch input { opacity: 0; width: 0; height: 0; }
.slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s; border-radius: 34px; }
.slider:before { position: absolute; content: ""; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s; border-radius: 50%; }
input:checked + .slider { background-color: #0cd334ff; }
input:checked + .slider:before { transform: translateX(26px); }
)rawliteral";

const char html_head[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="/style.css">
</head><body>
<h1>LED Control</h1>
<label class="switch">
<input type="checkbox" onchange="toggle(this)"
)rawliteral";

const char html_tail[] PROGMEM = R"rawliteral(
><span class="slider"></span></label>
<script>
function toggle(cb) { fetch(cb.checked ? '/led/on' : '/led/off'); }
</script>
</body></html>
)rawliteral";

#endif
