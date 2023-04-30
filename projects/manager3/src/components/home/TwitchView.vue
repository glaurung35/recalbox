<template>
  <div class="">
    test
  </div>
</template>

<script lang="ts" setup>
import { useTwitchStore } from 'stores/twitch';

const twitchStore = useTwitchStore();
twitchStore.getSchedule();
</script>

<style lang="sass" scoped>

</style>
