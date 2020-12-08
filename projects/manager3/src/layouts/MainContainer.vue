<template>
  <q-layout container style="height: 100vh" view="hHh Lpr lff">
    <q-header class="mobile-only">
      <q-toolbar>
        <q-btn @click="leftDrawerOpen = !leftDrawerOpen" flat icon="mdi-menu"/>
      </q-toolbar>
    </q-header>

    <q-drawer
      :breakpoint="500"
      :mini="miniState"
      :width="200"
      @mouseout="miniState = true"
      @mouseover="miniState = false"
      class="menu"

      mini-to-overlay
      show-if-above
      v-model="leftDrawerOpen"
    >
      <q-list padding>
        <MenuLink
          :key="link.title"
          v-bind="link"
          v-for="link in menuLinks"
        />

        <q-separator/>

        <LangSelector/>
      </q-list>
      <div class="menu-logo">
        <a href="" title="Recalbox"><img alt="Recalbox logo mini" src="../assets/logo-boutons.svg"></a>
      </div>
    </q-drawer>

    <q-page-container style="height: 100%;">

      <router-view/>
      <ConfirmDialog/>
      <UploadModule/>
      <VirtualDevicesButton/>
      <HelpButton/>
    </q-page-container>
  </q-layout>
</template>

<script>
  import HelpButton from 'components/global/HelpButton'
  import LangSelector from 'components/global/LangSelector'
  import MenuLink from 'components/global/MenuLink'
  import UploadModule from 'components/global/UploadModule'
  import ConfirmDialog from 'components/global/ConfirmDialog'
  import VirtualDevicesButton from 'components/global/VirtualDevicesButton'

  export default {
    name: 'MainContainer',
    components: {
      VirtualDevicesButton,
      UploadModule,
      MenuLink,
      LangSelector,
      HelpButton,
      ConfirmDialog,
    },
    data() {
      return {
        leftDrawerOpen: false,
        menuLinks: [
          {
            title: 'menu.home',
            icon: 'mdi-view-dashboard',
            route: 'home',
            exact: true,
          },
          {
            title: 'menu.monitoring',
            icon: 'mdi-chart-bar-stacked',
            route: 'monitoring',
          },
          {
            title: 'menu.settings',
            icon: 'mdi-settings',
            route: 'settings',
          },
          {
            title: 'menu.emulation',
            icon: 'mdi-google-controller',
            route: 'emulation',
          },
        ],
        miniState: true,
      }
    },
  }
</script>

<style lang="sass">
  header
    button
      i
        color: $secondary

  .q-page-container
    background: $rc-light-grey
</style>
