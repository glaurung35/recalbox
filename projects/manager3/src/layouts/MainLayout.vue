<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-layout container style="height: 100vh"  view="lHh Lpr lFf">
    <q-header>
      <q-toolbar>
        <q-btn @click="leftDrawerOpen = !leftDrawerOpen" flat icon="mdi-menu"/>
        <q-toolbar-title />
        <MobileScreenshotButton />
        <!-- <MobileVirtualDevicesButton /> -->
        <MobileShortcutsButton :model="shortcutsButtonOpeningStatus" @toggle="toggle" />
        <MobileHelpButton :model="helpButtonOpeningStatus" @toggle="toggle" />
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
          v-for="link in menuLinks"
          :key="link.title"
          v-bind="link"
        />
        <q-separator/>
        <LangSelector/>
      </q-list>
      <div class="menu-logo">
        <a :href="websiteUrl" target="_blank">
          <img alt="Recalbox" src="../assets/logo-boutons.svg">
        </a>
      </div>
    </q-drawer>

    <q-page-container>
      <router-view />
      <q-page-sticky :offset="[18, 18]" position="bottom-right" class="desktop-only">
        <div class="help-menu-container">
          <ScreenshotFloatingButton />
          <!-- <VirtualDevicesFloatingButton /> -->
          <ShortcutsFloatingButton :model="shortcutsButtonOpeningStatus" @toggle="toggle" />
          <HelpFloatingButton :model="helpButtonOpeningStatus" @toggle="toggle" />
        </div>
      </q-page-sticky>
    </q-page-container>
  </q-layout>
</template>

<script lang="ts" setup>
import MobileHelpButton from 'components/layout/MobileHelpButton.vue';
import MobileScreenshotButton from 'components/layout/MobileScreenshotButton.vue';
import MobileShortcutsButton from 'components/layout/MobileShortcutsButton.vue';
// import MobileVirtualDevicesButton from 'components/layout/MobileVirtualDevicesButton.vue';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useSystemsStore } from 'stores/systems';
import { GlobalMenuLink } from 'stores/types/misc';
import { ref } from 'vue';
import ShortcutsFloatingButton from 'components/layout/ShortcutsFloatingButton.vue';
import MenuLink from 'components/ui-kit/MenuLink.vue';
import LangSelector from 'components/layout/LangSelector.vue';
// import VirtualDevicesFloatingButton from 'components/layout/VirtualDevicesFloatingButton.vue';
import ScreenshotFloatingButton from 'components/layout/ScreenshotFloatingButton.vue';
import HelpFloatingButton from 'components/layout/HelpFloatingButton.vue';

const leftDrawerOpen = ref<boolean>(false);
const miniState = ref<boolean>(true);
const menuLinks: GlobalMenuLink[] = [
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
    icon: 'mdi-cog',
    route: 'settings',
  },
  {
    title: 'menu.emulation',
    icon: 'mdi-gamepad-outline',
    route: 'emulation',
  },
  {
    title: 'menu.screenshots',
    icon: 'mdi-camera-iris',
    route: 'screenshots',
  },
  {
    title: 'menu.patreon',
    icon: 'mdi-patreon',
    route: 'patreon',
  },
];
const websiteUrl = process.env.WEBSITE_URL;
const shortcutsButtonOpeningStatus = ref(false);
const helpButtonOpeningStatus = ref(false);

useSystemsStore().fetch().then(() => {
  const emulationStationStore = useEmulationstationStore();
  const { currentState } = emulationStationStore;
  if (currentState.currentSystem === null) {
    emulationStationStore.fetchStatus();
  }
});

function toggle(event: { label: string; value: boolean; }) {
  if (event.label === 'shortcutsButton') {
    shortcutsButtonOpeningStatus.value = event.value;
    helpButtonOpeningStatus.value = false;
    return;
  }
  if (event.label === 'helpButton') {
    helpButtonOpeningStatus.value = event.value;
    shortcutsButtonOpeningStatus.value = false;
  }
}
</script>

<style lang="sass">
header
  button
    i
      color: $secondary

.q-page-container
  background: $rc-light-grey

  .q-page-sticky
    z-index: 200

.menu
  background: $primary

  .menu-logo
    position: absolute
    bottom: 0
    left: 0
    right: 0
    text-align: center

    img
      width: 30px
      margin-bottom: 1em

.help-menu-container
  background-color: rgba($primary, .8)
  border-radius: 4px

@media(min-width: 500px)
  .q-header
    display: none

@media(max-width: 500px)
  .q-page-sticky
    display: none
</style>
