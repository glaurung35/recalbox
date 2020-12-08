<template>
  <q-page class="background settings">
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
        <q-route-tab :to="{name: 'system'}" :label="$t('settings.menu.system')" icon="mdi-server" name="system"/>
        <q-route-tab :to="{name: 'audio'}" :label="$t('settings.menu.audio')" icon="mdi-volume-high" name="audio"/>
        <q-route-tab :to="{name: 'network'}" :label="$t('settings.menu.network')" icon="mdi-wifi" name="network"/>
        <q-route-tab :to="{name: 'emustation'}" icon="icon-emustation" label="EmulationStation" name="emustation" style="text-transform: none;"/>
        <q-route-tab :to="{name: 'kodi'}" icon="mdi-kodi" label="Kodi" name="kodi"/>
      </q-tabs>
    </transition>

    <q-tab-panels animated v-model="tab">
      <q-tab-panel name="system">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="audio">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="network">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="emustation">
        <router-view/>
      </q-tab-panel>

      <q-tab-panel name="kodi">
        <router-view/>
      </q-tab-panel>

    </q-tab-panels>
  </q-page>
</template>

<script>
  export default {
    name: 'Settings',
    created() {
      this.$store.dispatch('system/options')
      this.$store.dispatch('updates/options')
      this.$store.dispatch('audio/options')
      this.$store.dispatch('emulationstation/options')
    },
    data() {
      return {
        tab: 'system',
      }
    },
  }
</script>

<style lang="sass">
  .settings
    &:before
      content: "\F493"

    .q-tabs
      background: white

    .q-tab-panels
      background: transparent
</style>
