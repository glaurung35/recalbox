<template>
  <q-page-sticky :offset="[18, 18]" position="bottom-right" style="transform: translate(-74px, 0px);">
    <q-btn
      padding="16px"
      flat
      square
      icon="mdi-keyboard"
      color="primary"
      id="virtual-devices-button"
      @click="show(true)"
    />
<!--    <q-fab-->
<!--      color="primary"-->
<!--      direction="up"-->
<!--      flat-->
<!--      icon="mdi-keyboard"-->
<!--      id="virtual-devices-button"-->
<!--      square-->
<!--      vertical-actions-align="right"-->
<!--    >-->
<!--      <q-fab-action-->
<!--        :href="routes.keyboard"-->
<!--        color="secondary"-->
<!--        icon="mdi-keyboard-outline"-->
<!--        label="Clavier"-->
<!--        label-position="left"-->
<!--        square-->
<!--        target="_blank"-->
<!--      />-->
<!--      <q-fab-action-->
<!--        :href="routes.gamepad"-->
<!--        color="secondary"-->
<!--        icon="mdi-gamepad-variant-outline"-->
<!--        label="Manette"-->
<!--        label-position="left"-->
<!--        square-->
<!--        target="_blank"-->
<!--      />-->
<!--      <q-fab-action-->
<!--        :href="routes.trackpad"-->
<!--        color="secondary"-->
<!--        icon="mdi-trackpad"-->
<!--        label="Trackpad"-->
<!--        label-position="left"-->
<!--        square-->
<!--        target="_blank"-->
<!--      />-->
<!--    </q-fab>-->
  </q-page-sticky>
</template>

<script>
export default {
  name: "VirtualDevicesButton",
  data() {
    return {
      routes: {
        keyboard: '/virtual-devices/keyboard',
        gamepad: '/virtual-devices/gamepad',
        trackpad: '/virtual-devices/trackpad'
      }
    }
  },
  methods: {
    show (grid) {
      this.$q.bottomSheet({
        message: this.$t('general.virtualDevices.title'),
        grid,
        dark: true,
        class: 'virtual-devices-menu',
        actions: [
          {
            label: this.$t('general.virtualDevices.keyboard'),
            icon: 'mdi-keyboard',
            id: 'keyboard',
            route: 'virtual-devices-keyboard',
          },
          {
            label: this.$t('general.virtualDevices.gamepad'),
            icon: 'mdi-gamepad-variant-outline',
            id: 'gamepad',
            route: 'virtual-devices-gamepad',
          },
          {
            label: this.$t('general.virtualDevices.trackpad'),
            icon: 'mdi-trackpad',
            id: 'trackpad',
            route: 'virtual-devices-trackpad',
          },
        ]
      }).onOk(button => {
        let routeData = this.$router.resolve({name: button.route})
        window.open(routeData.href, '_blank')
      })
    }
  }
}
</script>

<style lang="sass">
#virtual-devices-button
  border-radius: 4px

.virtual-devices-menu
  width: inherit

  .q-bottom-sheet__item
    min-width: 140px
</style>
