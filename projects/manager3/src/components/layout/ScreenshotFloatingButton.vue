<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page-sticky
    position="bottom"
    style="transform: translate(-132px, 0px);"
  >
    <q-btn
      padding="16px"
      flat
      square
      icon="mdi-camera-plus"
      color="primary"
      id="virtual-devices-button"
      @click="useMediaStore().takeScreenshot()"
    >
      <q-tooltip class="bg-primary" anchor="top middle" self="bottom middle" :offset="[10, 10]">
        {{ $t('emulation.screenshots.tooltip') }}
      </q-tooltip>
    </q-btn>
  </q-page-sticky>
</template>

<script lang="ts" setup>
import { useMediaStore } from 'stores/media';
</script>
