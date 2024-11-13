Component({
  data: {
    items: [
      { title: "mqtt", path: "/pages/esp-now/index" },
      { title: "ble", path: "/pages/esp-now/index" },
      { title: "esp-now", path: "/pages/esp-now/index" },
      { title: "bt", path: "/pages/bt/search/search" }
    ]
  },
  methods: {
    hanldeItemClick(event) {
      const { path } = event.currentTarget.dataset;
      wx.navigateTo({
        url: path,
      })
    }
  }
})
