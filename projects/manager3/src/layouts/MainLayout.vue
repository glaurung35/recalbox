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
      dark
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
        <a href="" title="Recalbox">
          <img alt="Recalbox logo mini" src="../assets/logo-boutons.svg">
        </a>
      </div>
    </q-drawer>

    <q-page-container>
      <router-view />
      <ScreenshotButton/>
      <VirtualDevicesButton/>
      <HelpButton/>
    </q-page-container>
  </q-layout>
</template>

<script lang="ts" setup>
import { ref } from 'vue';
import HelpButton from 'components/global/HelpButton.vue';
import MenuLink from 'components/global/MenuLink.vue';
import LangSelector from 'components/global/LangSelector.vue';
import VirtualDevicesButton from 'components/global/VirtualDevicesButton.vue';
import ScreenshotButton from "components/global/ScreenshotButton.vue";

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
];
</script>

<style lang="sass">
header
  button
    i
      color: $secondary

.q-page-container
  background: $rc-light-grey

.menu
  .menu-logo
    position: absolute
    bottom: 0
    left: 0
    right: 0
    text-align: center

    img
      width: 30px
      margin-bottom: 1em
</style>
