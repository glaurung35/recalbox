<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div :class="classes">
    <q-icon :name="icon"/>
  </div>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';

const props = defineProps({
  background: { type: Boolean },
  icon: { type: String, required: true },
});

const {
  background,
  icon,
} = toRefs(props);

const classes = computed(() => `overlayMessage${background.value ? ' background' : ''}`);
</script>

<style lang="sass">
.background
  background: #F4F5F7BA

.overlayMessage
  position: absolute
  left: 0
  top: -1em
  width: 100%
  height: calc(100% + 1em)
  z-index: 100
  display: flex
  align-items: center
  justify-content: center

  .q-icon
    color: $light-blue
    font-size: 9em
</style>
