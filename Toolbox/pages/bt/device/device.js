const app = getApp()
const apertureList = [18, 20, 22, 28, 32, 40, 56, 80, 110, 160]; // 光圈列表 除以10
const shutterSpeedList = [1, 2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000, 4000]; // 快门速度列表 (单位: n分之1/秒)

// 当前光圈和快门速度的索引
let currentApertureIndex = 0;
let currentShutterSpeedIndex = 0;

// 获取下一个光圈值
function nextAperture() {
  currentApertureIndex = (currentApertureIndex + 1) % apertureList.length; // 循环列表
  const apertureValue = apertureList[currentApertureIndex];
  console.log(`当前光圈值: f/${apertureValue}`);
  return apertureValue;
}

// 获取下一个快门速度值
function nextShutterSpeed() {
  currentShutterSpeedIndex = (currentShutterSpeedIndex + 1) % shutterSpeedList.length; // 循环列表
  const shutterSpeedValue = shutterSpeedList[currentShutterSpeedIndex];
  console.log(`当前快门速度: 1/${shutterSpeedValue} 秒`);
  return shutterSpeedValue;
}


Page({
  data: {
    title: "测光笔",
    inputText: 'mootek',
    receiveText: '',
    name: '',
    connectedDeviceId: '',
    services: [],
    characteristics: [],
    connected: true,
    serviceId: "0000FEA6-0000-1000-8000-00805F9B34FB",
    apertureCharId: '0000FEA7-0000-1000-8000-00805F9B34FB', // 光圈控制特性 UUID
    shutterCharId: '0000FEA8-0000-1000-8000-00805F9B34FB', // 快门控制特性 UUID
    photoCharId: 'B5F90072-AA8D-11E3-9046-0002A5D5C51B', // 拍照特性 UUID
    isoCharId: "0000FEA9-0000-1000-8000-00805F9B34FB",
    aperture: "f/1.2",
    shutter: "1/4000",
    iso: "ISO 100",
  },
  onLoad(options) {
    const { connectedDeviceId, name } = options;
    this.setData({
      name: decodeURIComponent(name),
      connectedDeviceId,
    });
    // 获取蓝牙服务
    this.getBLEDeviceServices();
    wx.onBLEConnectionStateChange((res) => {
      console.log("蓝牙连接状态:", res.connected);
      this.setData({ connected: res.connected });
    });
  },
  // 获取蓝牙服务
  getBLEDeviceServices() {
    const { connectedDeviceId } = this.data;
    wx.getBLEDeviceServices({ deviceId: connectedDeviceId })
    .then( (res) => {
      console.log("服务列表:", res.services);
      this.setData({ services: res.services });
      this.getCharacteristics(res.services[0].uuid);
    })
    .catch((err) => {
      console.error("获取服务失败:", err);
    })
  },
  // 获取特征值
  getCharacteristics(serviceId) {
    const { connectedDeviceId } = this.data;
    wx.getBLEDeviceCharacteristics({ deviceId: connectedDeviceId, serviceId })
    .then((res) => {
      console.log("特征值列表:", res.characteristics);
      this.setData({ characteristics: res.characteristics });
      // 启用需要监听的特征值
      this.enableNotify(this.data.isoCharId);
      this.enableNotify(this.data.apertureCharId);
      this.enableNotify(this.data.shutterCharId);
    })
    .catch((err) => {
      console.error("获取特征值失败:", err);
    });
  },
  // 启用 Notify
  enableNotify(characteristicId) {
    const { connectedDeviceId, services } = this.data;
    wx.notifyBLECharacteristicValueChange({
      state: true,
      deviceId: connectedDeviceId,
      serviceId: services[0].uuid,
      characteristicId
    })
    .then(() => {
      console.log(`启用通知成功：${characteristicId}`);
    })
    .catch((err) => {
      console.error(`启用通知失败：${characteristicId}`, err);
    });
  },

  // 写入特征值
  writeCharacteristic(characteristicId, buffer) {
    const { connectedDeviceId, serviceId } = this.data;
    wx.writeBLECharacteristicValue({ deviceId: connectedDeviceId, serviceId, characteristicId, value: buffer })
    .then(() => {
      console.log(`写入特征值成功：${characteristicId}`);
    })
    .catch((err) => {
      console.error(`写入特征值失败：${characteristicId}`, err);
    });
  },

  // 设置光圈
  setAperture(apertureValue) {
    const buffer = new ArrayBuffer(1);
    new DataView(buffer).setUint8(0, apertureValue);
    this.writeCharacteristic(this.data.apertureCharId, buffer);
    console.log(`光圈设置为 f/${apertureValue / 10}`);
  },
  // 设置快门速度
  setShutterSpeed(shutterSpeed) {
    const buffer = new ArrayBuffer(2);
    const dataView = new DataView(buffer);
    dataView.setUint8(0, Math.floor(shutterSpeed / 100));
    dataView.setUint8(1, shutterSpeed % 100);
    this.writeCharacteristic(this.data.shutterCharId, buffer);
    console.log(`快门速度设置为 1/${shutterSpeed} 秒`);
  },
  // 拍照
  takePhoto() {
    const buffer = new ArrayBuffer(2);
    const dataView = new DataView(buffer);
    dataView.setUint8(0, 0x03);
    dataView.setUint8(1, 0x01);
    this.writeCharacteristic(this.data.photoCharId, buffer);
    console.log("拍照指令已发送");
  },
  handleNextAperture(){
    this.setAperture(nextAperture());
  },
  handleNextShutterSpeed(){
    this.setShutterSpeed(nextShutterSpeed());
  },
  handleTakePhoto(){
    this.takePhoto();
  },
  onReady: function () {
    wx.onBLECharacteristicValueChange((res) => {
      const receiveText = app.buf2string(res.value);
      console.log("特征值变化:", receiveText);
      if(res.characteristicId === this.data.apertureCharId){
        this.setData({ aperture: receiveText });
      } else if (res.characteristicId === this.data.isoCharId){
        this.setData({ iso: receiveText });
      } else if (res.characteristicId === this.data.shutterCharId){
        this.setData({ shutter: receiveText });
      }
    });
  }
});
