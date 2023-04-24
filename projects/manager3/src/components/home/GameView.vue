<template>
  <div class="header">
    <div class="title">Jeux en cours</div>
  </div>
  <div class="informations">
    <div class="screen">
      <img src="../../assets/game-exemple.png" alt="Hokuto No Ken">
    </div>
  </div>
</template>

<script lang="ts" setup>

</script>

<style lang="sass" scoped>
.header
  display: flex
  justify-content: space-between
  gap: 1em
  margin-bottom: 1em
  padding: 0.2em
  border-bottom: 2px solid white

  .title
    display: flex
    align-items: center
    text-transform: uppercase
    font-weight: 300
    color: $accent
    min-height: 36px
    margin-left: 1em

.informations
  display: flex
  justify-content: center
  padding: 0 1em

  .screen
    @keyframes flicker
      $steps:20
      @for $i from 0 through $steps
        #{percentage($i * ( 1 / $steps ))}
          opacity: random()

    $screen-background: #121010

    position: relative
    width: 70%
    border-bottom-left-radius: 100% 4%
    border-bottom-right-radius: 100% 4%
    border-top-left-radius: 100% 4%
    border-top-right-radius: 100% 4%

    // flicker
    &::after
      content: ' '
      display: block
      position: absolute
      top: 0
      left: 0
      bottom: 0
      right: 0
      background: transparentize($screen-background,0.9)
      opacity: 0
      z-index: 2
      pointer-events: none
      animation: flicker 0.15s infinite
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%

    // scanlines
    &::before
      content: ' '
      display: block
      position: absolute
      top: 0
      left: 0
      bottom: 0
      right: 0
      background: linear-gradient(transparentize($screen-background,1) 50%, transparentize(darken($screen-background,10),0.75) 50%), linear-gradient(90deg,transparentize(#ff0000,0.94), transparentize(#00ff00,0.98),transparentize(#0000ff,0.94))
      z-index: 2
      background-size: 100% 2px, 3px 100%
      pointer-events: none
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%

    img
      display: block
      width: 100%
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%
      opacity: 0.6
</style>
