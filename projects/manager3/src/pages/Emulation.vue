<template>
  <q-page class="background emulation">
    <transition
      appear
      enter-active-class="animated slideInDown"
    >
      <q-tabs
        active-color="primary"
        align="justify"
        class="text-grey"
        dense
        indicator-color="accent"
        inline-label
        narrow-indicator
      >
        <q-route-tab :to="{name: 'global'}" :label="$t('emulation.menu.global')" icon="mdi-settings-outline" name="global"/>
        <q-route-tab :to="{name: 'bios'}" :label="$t('emulation.menu.bios')" icon="mdi-heart-pulse" name="bios"/>
        <q-route-tab :to="{name: 'systems'}" :label="$t('emulation.menu.systemsRoms')" icon="mdi-television-classic" name="systems"/>
        <q-route-tab :to="{name: 'controllers'}" :label="$t('emulation.menu.controllers')" icon="mdi-gamepad-variant-outline" name="controllers"/>
        <q-route-tab :to="{name: 'screenshots'}" :label="$t('emulation.menu.screenshots')" icon="mdi-camera-iris" name="screenshots"/>
      </q-tabs>
    </transition>

    <q-tab-panels
      v-model="tab"
      animated
    >
      <q-tab-panel name="global">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="bios">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="systems">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="controllers">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="screenshots">
        <router-view/>
      </q-tab-panel>

    </q-tab-panels>
  </q-page>
</template>

<script>
  export default {
    name: 'Emulation',
    created() {
      this.$store.dispatch('global/options')
      this.$store.dispatch('controllers/options')
    },
    data() {
      return {
        tab: 'global',
      }
    },
  }
</script>

<style lang="sass">
  .emulation
    &:before
      content: "\F2B4"

    .q-tabs
      background: white

    .q-tab-panels
      background: transparent
</style>
