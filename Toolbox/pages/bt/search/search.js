const app = getApp()
Page({
  data: {
    title: "测光笔",
    list_height: wx.getWindowInfo().safeArea.height - 88,
    searching: false,
    devicesList: []
  },
  handleSearch() {
    if (!this.data.searching) {
      wx.closeBluetoothAdapter()
        .then(console.log)
        .catch(console.log)
      wx.openBluetoothAdapter()
        .then(() => {
          return wx.getBluetoothAdapterState().then(console.log)
        })
        .then(() => wx.startBluetoothDevicesDiscovery())
        .then((res) => {
          console.log(res)
          this.setData({
            searching: true,
            devicesList: []
          })
        }).catch((err) => {
          console.log(err)
          wx.showModal({
            title: '提示',
            content: '请检查手机蓝牙是否打开',
            showCancel: false,
            success(res) {
              wx.openAppAuthorizeSetting().then(console.log).catch(console.log)
              // wx.openSystemBluetoothSetting().then(console.log).catch(console.log)
              this.setData({
                searching: false
              })
            }
          })
        })
    } else {
      wx.stopBluetoothDevicesDiscovery().then(res => {
        console.log(res)
        this.setData({
          searching: false
        })
      })
    }
  },
  handleConnect(e) {
    wx.stopBluetoothDevicesDiscovery({
      success: (res) => {
        console.log(res)
        this.setData({
          searching: false
        })
      }
    })
    wx.showLoading({
      title: '连接蓝牙设备中...',
    })
    const { id, dataset } = e.currentTarget;
    wx.createBLEConnection({
      deviceId: id,
      success(res) {
        console.log("连接蓝牙设备成功" + res)
        wx.hideLoading()
        wx.showToast({
          title: '连接成功',
          icon: 'success',
          duration: 1000
        })
        wx.navigateTo({
          url: '../device/device?connectedDeviceId=' + e.currentTarget.id + '&name=' + encodeURIComponent(dataset.name)
        })
      },
      fail(res) {
        console.log(res)
        wx.hideLoading()
        wx.showModal({
          title: '提示',
          content: '连接失败',
          showCancel: false
        })
      }
    })
  },
  onLoad() {
    wx.authorize({ scope: 'scope.bluetooth' })
      .then(({ errMsg }) => {
        if (errMsg !== 'authorize:ok') {
          return wx.openSetting()
        }
      }).catch(console.log)

    wx.onBluetoothAdapterStateChange((res) => {
      console.log(res)
      this.setData({
        searching: res.discovering
      })
      if (!res.available) {
        this.setData({
          searching: false
        })
      }
    })

    wx.onBluetoothDeviceFound((devices) => {
      devices.devices
        .filter(v => v.name.trim())
        .forEach(newDevice => {
          // 转换 `advertisData` 为 Hex 字符串
          newDevice.advertisData = newDevice.advertisData
            ? app.buf2hex(newDevice.advertisData)
            : '';
          console.log("发现新设备:", newDevice);
          // 检查设备是否已存在于列表中
          const isExist = this.data.devicesList.some(
            (item) => item.deviceId === newDevice.deviceId
          );
          // 不存在则添加到设备列表
          if (!isExist) {
            this.data.devicesList.push(newDevice);
            this.setData({
              devicesList: this.data.devicesList,
            });
          }
        })
    });
  },
  onHide() {
    this.setData({
      devicesList: []
    })
    if (this.data.searching) {
      wx.stopBluetoothDevicesDiscovery().then((res) => {
        console.log(res)
        this.setData({
          searching: false
        })
      })
    }
  }
})