<template>
  <q-page-sticky :offset="[18, 18]" position="bottom-right" style="transform: translate(-74px, 0px);">
    <q-fab
      color="primary"
      direction="up"
      flat
      icon="mdi-keyboard"
      id="virtual-devices-button"
      square
      vertical-actions-align="right"
    >
      <q-fab-action
        :href="routes.keyboard"
        color="secondary"
        icon="mdi-keyboard-outline"
        label="Clavier"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="routes.gamepad"
        color="secondary"
        icon="mdi-gamepad-variant-outline"
        label="Manette"
        label-position="left"
        square
        target="_blank"
      />
      <q-fab-action
        :href="routes.trackpad"
        color="secondary"
        icon="mdi-trackpad"
        label="Trackpad"
        label-position="left"
        square
        target="_blank"
      />
    </q-fab>
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
  }
}
</script>

<style lang="sass">
.q-page-container
  #virtual-devices-button
    .q-btn--fab-mini:hover
      .q-icon:before
        color: $accent

    .gitbook
      i
        font-size: 18px
        margin-left: 3px
        margin-right: 3px
</style>
