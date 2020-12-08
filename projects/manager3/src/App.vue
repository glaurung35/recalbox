<template>
  <div id="q-app">
    <router-view/>
  </div>
</template>

<script>
  const myIcons = {
    // 'emustation': 'img:/statics/icons/emustation-icon.svg',
    // exemple => prefer using icon fonts like in assets/fonts
  }

  export default {
    name: 'App',
    created() {
      this.$q.iconMapFn = (iconName) => {
        const icon = myIcons[iconName]
        if (icon !== void 0) {
          return {icon: icon}
        }
      }
    },
  }
</script>
