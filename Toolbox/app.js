App({
  buf2hex: function (buffer) {
    return Array.prototype.map.call(new Uint8Array(buffer), x => ('00' + x.toString(16)).slice(-2)).join('')
  },
  buf2string: function (buffer) {
    var arr = Array.prototype.map.call(new Uint8Array(buffer), x => x)
    var str = ''
    for (var i = 0; i < arr.length; i++) {
      str += String.fromCharCode(arr[i])
    }
    return str
  },
  //将字符串转为Uint8Array数组
  stringToUint8Array: function (str) {
  let buffer = new ArrayBuffer(str.length)
  let mydataViewUint8Array = new Uint8Array(buffer)
  for (var i = 0; i < str.length; i++) {
    mydataViewUint8Array[i] = str.charCodeAt(i)
  }
  // console.log(mydataViewUint8Array)
  return mydataViewUint8Array;
},
bleSerialBroadcastCmdUint8Array: function (str) {
  let length = str.length + 1;
  let bytesCmdUint8Array = new Uint8Array([251, 27, 0, (length % 256), parseInt((length / 256)), ...this.stringToUint8Array(str), 254]);
  return bytesCmdUint8Array;
},
uint8ArrayToArrayBuffer: function (uint8Array) {
  const arrayBuffer = uint8Array.buffer.slice(uint8Array.byteOffset, uint8Array.byteOffset + uint8Array.byteLength);
  return arrayBuffer;
},
  onLaunch: function () {
    this.globalData.SystemInfo = wx.getSystemInfoSync()
    console.log(this.globalData.SystemInfo)
  },
  globalData: {
    SystemInfo: {}
  }
})