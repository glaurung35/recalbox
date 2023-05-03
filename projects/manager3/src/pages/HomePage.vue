<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page class="background home">
    <div class="header">
      <div class="background-container">
        <img src="../assets/recalbox.svg" alt="Recalbox" />
      </div>
    </div>
    <div class="row stats">
      <div class="vertical-middle text-center preview self-center">
        {{ $t('home.preview.title') }}
        <span class="arrow-right"><q-icon name="mdi-chevron-right"/></span>
        <span class="arrow-bottom"><q-icon name="mdi-chevron-down"/></span>
      </div>
      <q-separator vertical/>
      <q-separator/>

      <TotalStat
        :key="stat.key"
        v-bind="stat"
        v-for="stat in totals"
      />
      <SystemStat
        :key="stat.key"
        v-bind="stat"
        v-for="stat in percents"
      />
    </div>

    <div class="row flex items-stretch emustation">
      <div class="col social-medias">
        <TwitchView/>
      </div>
      <div class="col system">
        <SystemView/>
      </div>
      <div class="col game">
        <GameView/>
      </div>
    </div>
  </q-page>
</template>

<script lang="ts" setup>
import { useMonitoringStore } from 'src/stores/monitoring';
import { computed } from 'vue';
import SystemStat from 'components/ui-kit/PercentStat.vue';
import TotalStat from 'components/ui-kit/TotalStat.vue';
import { useSystemsStore } from 'stores/systems';
import SystemView from 'components/home/SystemView.vue';
import GameView from 'components/home/GameView.vue';
import TwitchView from 'components/home/TwitchView.vue';

const { getSystemsListCount } = useSystemsStore();
useSystemsStore().fetch();

const { getSharePercent } = useMonitoringStore();
useMonitoringStore().fetch();

const totals = computed<object[]>(() => [
  {
    key: 1,
    title: 'home.preview.systems',
    value: getSystemsListCount(),
    route: 'systems-parent',
  },
  {
    key: 2,
    title: 'home.preview.roms',
    value: 70543,
  },
]);

const percents = computed<object[]>(() => [
  {
    key: 3,
    title: 'home.preview.sharePercent',
    value: getSharePercent(),
    percent: getSharePercent(),
  },
]);
</script>

<style lang="sass" scoped>
.home
  height: 100%

  &:before
    content: "\F056E"

  .header
    height: 5em
    background-image: linear-gradient(to right, $primary, $accent)
    -webkit-box-shadow: inset 0 0 15px 4px rgba(0,0,0,0.29)
    box-shadow: inset 0 0 15px 4px rgba(0,0,0,0.29)

    .background-container
      height: 100%
      background-image: url(../assets/controller-background.png)
      position: relative

      @keyframes logoslidein
        from
          right: -38rem
        to
          right: 1em

      img
        max-width: 310px
        position: absolute
        right: 1em
        bottom: 0
        animation-duration: .5s
        animation-name: logoslidein

  .stats
    background: white
    min-height: 212px
    position: relative

    .preview
      text-transform: uppercase
      font-weight: 100
      font-size: x-large
      color: $primary

    @media(max-width: 1023px)
      .preview
        width: 100%

      .arrow-right,
      .q-separator--vertical
        display: none

    @media(min-width: 1024px)
      .preview
        width: 15%

      .arrow-bottom,
      .q-separator--horizontal
        display: none

  .q-separator--vertical
    background: $rc-light-grey
    width: 2px
    min-width: 2px
    margin-bottom: 0

  .q-separator--horizontal
    background: $rc-light-grey
    width: 100%
    height: 2px

  .emustation
    height: calc(100vh - 212px - 5em)

    .social-medias,
    .system,
    .game
      position: relative
      display: flex
      flex-direction: column

    .system
      background: $rc-light-grey
      border-right: 2px solid white

      &:after
        display: block
        content: ''
        border-top: 2px solid white
        border-right: 2px solid white
        height: 50px
        width: 50px
        transform: rotate(45deg) translateY(-50%)
        position: absolute
        right: -8px
        background: $rc-light-grey
        margin-top: calc(50vh - 131px)
        z-index: 1

    .social-medias
      flex-basis: 20%
      background: white

    .system,
    .game
      flex-basis: 40%

    @media(max-width: 1400px)
      .social-medias
        display: none

@media(max-width: 950px)
  .home
    overflow-y: auto

    .emustation
      .system,
      .game
        flex-basis: 100%

      .system
        padding-bottom: 2em

      .game
        border-top: 2px solid white
        padding-bottom: 10em

@media(max-width: 420px)
  .home
    .header
      .background-container
        img
          max-width: 200px

    .stats
      min-height: auto

      .q-separator--horizontal,
      .total,
      .percent
        display: none

    .emustation
      .system
        .header
          flex-direction: column

          .controls
            justify-content: space-between
</style>
