/*
 * More web UI HTML source arrays.
 * This file is auto generated, please don't make any changes manually.
 * Instead, see https://github.com/Aircoookie/WLED/wiki/Add-own-functionality#web-ui
 * to find out how to easily modify the web UI source!
 */

// Autogenerated from wled00/data/usermod.htm, do not edit!!
const char PAGE_usermod[] PROGMEM = R"=====(<!DOCTYPE html><html><body>No usermod custom web page set.</body></html>)=====";


// Autogenerated from wled00/data/msg.htm, do not edit!!
const char PAGE_msg[] PROGMEM = R"=====(<!DOCTYPE html><html><head><meta content="width=device-width" name="viewport">
<title>WLED Message</title><script>
function B(){window.history.back()}function RS(){window.location="/settings"}function RP(){top.location.href="/"}
</script><style>
.bt{background:#333;color:#fff;font-family:Verdana,sans-serif;border:.3ch solid #333;display:inline-block;font-size:20px;margin:8px;margin-top:12px}body{font-family:Verdana,sans-serif;text-align:center;background:#222;color:#fff;line-height:200%%;margin:0}
</style></head><body><h2>%MSG%</body></html>)=====";


#ifdef WLED_ENABLE_DMX

// Autogenerated from wled00/data/dmxmap.htm, do not edit!!
const char PAGE_dmxmap[] PROGMEM = R"=====(<!DOCTYPE html><html><head><meta content="width=device-width" name="viewport">
<title>DMX Map</title><script>
function B(){window.history.back()}function RS(){window.location="/settings"}function RP(){top.location.href="/"}function FM() {%DMXVARS%
var t=["SET 0","RED","GREEN","BLUE","WHITE","SHUTTER","SET 255","DISABLED"],n=[];for(i=0;i<512;i++)n.push(7);for(i=0;i<LC;i++)for(FS=CS+CG*i,j=0;j<CN;j++)DA=FS+j,n[DA-1]=CH[j];for(DMXMap="",i=0;i<512;i++)isstart="",(i+1)%10==0&&(isstart="S"),DMXMap+='<div class="anytype '+isstart+" type"+n[i]+'">'+String(i+1)+"<br />"+t[n[i]]+"</div>";document.getElementById("map").innerHTML=DMXMap}
</script><style>
.anytype{border:1px solid #fff;margin:1px;float:left;width:100px;height:100px}.S{margin:0;border:2px solid #fff}.type7{color:#888;border:1px dotted grey}.type6{color:#fff}.type4{color:#fff;font-weight:700}.type3{color:#00f;font-weight:700}.type2{color:#0f0;font-weight:700}.type1{color:red;font-weight:700}.bt{background:#333;color:#fff;font-family:Verdana,sans-serif;border:.3ch solid #333;display:inline-block;font-size:20px;margin:8px;margin-top:12px}body{font-family:Verdana,sans-serif;text-align:center;background:#222;color:#fff;line-height:200%%;margin:0}
</style></head><body onload="FM()"><div id="map">...</div></body></html>)=====";


#else
const char PAGE_dmxmap[] PROGMEM = R"=====()=====";
#endif

// Autogenerated from wled00/data/update.htm, do not edit!!
const char PAGE_update[] PROGMEM = R"=====(<!DOCTYPE html><html><head><meta content="width=device-width" name="viewport">
<title>WLED Update</title><script>function B(){window.history.back()}</script>
<style>
.bt{background:#333;color:#fff;font-family:Verdana,sans-serif;border:.3ch solid #333;display:inline-block;font-size:20px;margin:8px;margin-top:12px}input[type=file]{font-size:16px}body{font-family:Verdana,sans-serif;text-align:center;background:#222;color:#fff;line-height:200%}
</style></head><body><h2>WLED Software Update</h2>Installed version: 0.10.1<br>
Download the latest binary: <a href="https://github.com/atuline/WLED/releases">
<img 
src="https://img.shields.io/github/release/Aircoookie/WLED.svg?style=flat-square">
</a><br><form method="POST" action="/update" enctype="multipart/form-data">
<input type="file" class="bt" name="update" required><br><input type="submit" 
class="bt" value="Update!"></form><button type="button" class="bt" 
onclick="B()">Back</button></body></html>)=====";


// Autogenerated from wled00/data/welcome.htm, do not edit!!
const char PAGE_welcome[] PROGMEM = R"=====(<!DOCTYPE html><html><head><meta charset="utf-8"><meta 
content="width=device-width" name="viewport"><meta name="theme-color" 
content="#333333"><title>WLED Setup</title><style>
body{font-family:Verdana,Helvetica,sans-serif;text-align:center;background-color:#333;margin:0;color:#fff}button{outline:0;cursor:pointer;padding:8px;margin:10px;width:230px;text-transform:uppercase;font-family:helvetica;font-size:19px;background-color:#222;color:#fff;border:0 solid #fff;border-radius:5px}svg{fill:#fff}
</style></head><body><svg 
style="position:absolute;width:0;height:0;overflow:hidden" version="1.1" 
xmlns="http://www.w3.org/2000/svg"><defs><symbol id="lnr-smile" 
viewBox="0 0 1024 1024"><path 
d="M486.4 1024c-129.922 0-252.067-50.594-343.936-142.464s-142.464-214.014-142.464-343.936c0-129.923 50.595-252.067 142.464-343.936s214.013-142.464 343.936-142.464c129.922 0 252.067 50.595 343.936 142.464s142.464 214.014 142.464 343.936-50.594 252.067-142.464 343.936c-91.869 91.87-214.014 142.464-343.936 142.464zM486.4 102.4c-239.97 0-435.2 195.23-435.2 435.2s195.23 435.2 435.2 435.2 435.2-195.23 435.2-435.2-195.23-435.2-435.2-435.2z">
</path><path 
d="M332.8 409.6c-42.347 0-76.8-34.453-76.8-76.8s34.453-76.8 76.8-76.8 76.8 34.453 76.8 76.8-34.453 76.8-76.8 76.8zM332.8 307.2c-14.115 0-25.6 11.485-25.6 25.6s11.485 25.6 25.6 25.6 25.6-11.485 25.6-25.6-11.485-25.6-25.6-25.6z">
</path><path 
d="M640 409.6c-42.349 0-76.8-34.453-76.8-76.8s34.451-76.8 76.8-76.8 76.8 34.453 76.8 76.8-34.451 76.8-76.8 76.8zM640 307.2c-14.115 0-25.6 11.485-25.6 25.6s11.485 25.6 25.6 25.6 25.6-11.485 25.6-25.6-11.485-25.6-25.6-25.6z">
</path><path 
d="M486.4 870.4c-183.506 0-332.8-149.294-332.8-332.8 0-14.139 11.462-25.6 25.6-25.6s25.6 11.461 25.6 25.6c0 155.275 126.325 281.6 281.6 281.6s281.6-126.325 281.6-281.6c0-14.139 11.461-25.6 25.6-25.6s25.6 11.461 25.6 25.6c0 183.506-149.294 332.8-332.8 332.8z">
</path></symbol></defs></svg><br><br><svg><use xlink:href="#lnr-smile"></use>
</svg><h1>Welcome to WLED!</h1><h3>Thank you for installing my application!</h3>
If you encounter a bug or have a question/feature suggestion, feel free to open a GitHub issue!
<br><br><b>Next steps:</b><br><br>Connect the module to your local WiFi here!
<br><button onclick='window.location.href="/settings/wifi"'>WiFi settings
</button><br><i>Just trying this out in AP mode?</i><br><button 
onclick='window.location.href="/sliders"'>To the controls!</button></body>
</html>)=====";


// Autogenerated from wled00/data/liveview.htm, do not edit!!
const char PAGE_liveview[] PROGMEM = R"=====(<!DOCTYPE html><html><head><meta name="viewport" 
content="width=device-width,initial-scale=1,minimum-scale=1"><meta 
charset="utf-8"><meta name="theme-color" content="#222222"><title>
WLED Live Preview</title><style>
body{margin:0}#canv{background:#000;filter:brightness(175%);width:100%;height:100%;position:absolute}
</style></head><body><div id="canv"><script>update();
    
    var tmout = null;
    function update()
    {
      if (document.hidden) {
        clearTimeout(tmout);
        tmout = setTimeout(update, 250);
        return;
      }
      fetch('/json/live')
      .then(res => {
        if (!res.ok) {
          clearTimeout(tmout);
          tmout = setTimeout(update, 2500);
        }
        return res.json();
      })
      .then(json => {
        var str = "linear-gradient(90deg,";
        var len = json.leds.length;
        for (i = 0; i < len; i++) {
          var leddata = json.leds[i];
          if (leddata.length > 6) leddata = leddata.substring(2);
          str += "#" + leddata;
          if (i < len -1) str += ","
        }
        str += ")";
        document.getElementById("canv").style.background = str;
        clearTimeout(tmout);
        tmout = setTimeout(update, 40);
      })
      .catch(function (error) {
        clearTimeout(tmout);
        tmout = setTimeout(update, 2500);
      })

    }</script></body></html>)=====";


// Autogenerated from wled00/data/favicon.ico, do not edit!!
const uint16_t favicon_length = 954;
const uint8_t favicon[] PROGMEM = {
  0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x86, 0x00,
  0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00,
  0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x08, 0x06,
  0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00, 0x4d, 0x49, 0x44, 0x41, 0x54, 0x38,
  0x8d, 0x63, 0xfc, 0xff, 0xff, 0x3f, 0x03, 0xb1, 0x80, 0xd1, 0x9e, 0x01, 0x43, 0x31, 0x13, 0xd1,
  0xba, 0x71, 0x00, 0x8a, 0x0d, 0x60, 0x21, 0xa4, 0x00, 0xd9, 0xd9, 0xff, 0x0f, 0x32, 0x30, 0x52,
  0xdd, 0x05, 0xb4, 0xf1, 0x02, 0xb6, 0xd0, 0xa6, 0x99, 0x0b, 0x68, 0x1f, 0x0b, 0xd8, 0x42, 0x9e,
  0xaa, 0x2e, 0xa0, 0xd8, 0x00, 0x46, 0x06, 0x3b, 0xcc, 0xcc, 0x40, 0xc8, 0xd9, 0x54, 0x75, 0x01,
  0xe5, 0x5e, 0x20, 0x25, 0x3b, 0x63, 0x03, 0x00, 0x3e, 0xb7, 0x11, 0x5a, 0x8d, 0x1c, 0x07, 0xb4,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

