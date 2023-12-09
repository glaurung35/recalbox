<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="sleepMessage">
    <q-icon name="mdi-sleep"/>
  </div>
</template>

<script lang="ts" setup>

</script>

<style lang="sass">
.sleepMessage
  position: absolute
  left: 0
  top: -1em
  width: 100%
  height: calc(100% + 1em)
  background: #F4F5F7BA
  z-index: 9999
  display: flex
  align-items: center
  justify-content: center

  .q-icon
    color: $light-blue
    font-size: 17em
</style>
