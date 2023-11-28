<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="background virtual-trackpad">
    <div class="trackpad" v-on:mousemove="updateCoordinates"/>
  </div>
</template>

<script lang="ts" setup>
// eslint-disable-next-line  @typescript-eslint/no-unused-vars
let x = 0;
// eslint-disable-next-line  @typescript-eslint/no-unused-vars
let y = 0;

function updateCoordinates(event: object) {
  x = event.offsetX;
  y = event.offsetY;
}
</script>

<style lang="sass">
.virtual-trackpad
  height: 100vh
  display: flex
  align-items: center
  padding: 1em
  justify-content: center

  &:before
    content: "\F07F8"

  .trackpad
    background-color: rgba(0, 0, 0, 0.1)
    width: 90vw
    height: 90vh
    border-radius: 5px
    opacity: 0.5
</style>
