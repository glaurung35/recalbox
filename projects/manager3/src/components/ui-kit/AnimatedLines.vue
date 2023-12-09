<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="lines">
    <div
      v-for="(color, index) in colors"
      :class="'line ' + index"
      :key="color"
      :style="{ backgroundColor: color }"
    />
  </div>
</template>

<script lang="ts" setup>
import { toRefs } from 'vue';

const props = defineProps({
  colors: { type: Object, required: true },
});
const { colors } = toRefs(props);
</script>

<style lang="sass">
.lines
  position: absolute
  display: flex
  flex-direction: row
  top: -1em
  left: 15px
  width: 6%
  height: calc(100% + 1em)
  opacity: 0.3

  .line
    animation-name: grow
    width: 20%
    height: 0
    animation-duration: .4s
    animation-timing-function: ease-in
    border-bottom-left-radius: 5px
    border-bottom-right-radius: 5px
    animation-fill-mode: forwards

    @keyframes grow
      from
        height: 0
      to
        height: 110%

  .band1
    animation-delay: 250ms
  .band2
    animation-delay: 350ms
  .band3
    animation-delay: 450ms
  .band4
    animation-delay: 550ms
</style>
