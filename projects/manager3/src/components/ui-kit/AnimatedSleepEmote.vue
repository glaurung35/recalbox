<template>
  <div class="zzz zzz-zzz">Z</div>
  <div class="zzz zzz-zz">Z</div>
  <div class="zzz zzz-z">Z</div>
</template>

<script lang="ts" setup>

</script>

<style lang="sass" scoped>
.zzz
  animation-name: zzz
  animation-duration: 2s
  animation-timing-function: ease-out
  animation-iteration-count: infinite
  color: $accent
  font-weight: bold
  z-index: 100
  transform: translateY(100%)

.zzz-z
  animation-delay: 0s
  right: 10px

.zzz-zz
  animation-delay: 0.5s
  right: -20px

.zzz-zzz
  animation-delay: 1s
  right: 0

@-webkit-keyframes zzz
  0%
    color: rgba(160,84,246,0)
    font-size: 30px
    -webkit-transform: translateY(100%)
    transform: translateY(100%)

  100%
    color: $accent
    font-size: 72px
    -webkit-transform: translateY(-100%)
    transform: translateY(-100%)

@keyframes zzz
  0%
    color: rgba(160,84,246,0)
    font-size: 30px
    -webkit-transform: translateY(100%)
    transform: translateY(100%)

  50%
    color: $accent
  100%
    color: rgba(160,84,246,0)
    font-size: 72px
    -webkit-transform: translateY(-100%)
    transform: translateY(-100%)
</style>
