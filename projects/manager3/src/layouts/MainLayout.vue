<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-layout container style="height: 100vh"  view="lHh Lpr lFf">
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
          v-for="link in menuLinks"
          :key="link.title"
          v-bind="link"
        />
        <q-separator/>
        <LangSelector/>
      </q-list>
      <div class="menu-logo">
        <img alt="Recalbox" src="../assets/logo-boutons.svg">
      </div>
    </q-drawer>

    <q-page-container>
      <router-view />
      <q-page-sticky :offset="[18, 18]" position="bottom-right">
        <div class="help-menu-container">
          <ScreenshotFloatingButton />
          <VirtualDevicesFloatingButton />
          <ShortcutsFloatingButton :model="shortcutsButtonOpeningStatus" @toggle="toggle" />
          <HelpFloatingButton :model="helpButtonOpeningStatus" @toggle="toggle" />
        </div>
      </q-page-sticky>
    </q-page-container>
  </q-layout>
</template>

<script lang="ts" setup>
import { ref } from 'vue';
import ShortcutsFloatingButton from 'components/layout/ShortcutsFloatingButton.vue';
import MenuLink from 'components/ui-kit/MenuLink.vue';
import LangSelector from 'components/layout/LangSelector.vue';
import VirtualDevicesFloatingButton from 'components/layout/VirtualDevicesFloatingButton.vue';
import ScreenshotFloatingButton from 'components/layout/ScreenshotFloatingButton.vue';
import HelpFloatingButton from 'components/layout/HelpFloatingButton.vue';

const leftDrawerOpen = ref<boolean>(false);
const miniState = ref<boolean>(true);
const menuLinks: Array<object> = [
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
    icon: 'mdi-google-controller',
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
const shortcutsButtonOpeningStatus = ref(false);
const helpButtonOpeningStatus = ref(false);

function toggle(event: object) {
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
  background-color: #34495ecf
  border-radius: 4px
</style>
