<template>
  <q-page class="background home">
    <div class="row home-stats">
      <div class="vertical-middle text-center preview-title self-center preview-title">
        {{ $t('home.preview.title') }}
        <span class="arrow-right"><q-icon name="mdi-chevron-right"/></span>
        <span class="arrow-bottom"><q-icon name="mdi-chevron-down"/></span>
      </div>
      <q-separator vertical/>
      <q-separator/>

      <!-- PC version -->
      <Stat
        :key="stat.key"
        v-bind="stat"
        v-for="stat in stats"
      />

      <!-- Mobile version -->
      <div class="col-12 linear-progress" style="padding: 16px">
        <div class="linear-progress-title text-accent">{{ $t('home.preview.systems') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.4" color="secondary" size="15px"/>
        </div>
        <div class="linear-progress-title text-accent">{{ $t('home.preview.roms') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.4" color="secondary" size="15px"/>
        </div>
        <div class="linear-progress-title text-accent">{{ $t('home.preview.sharePercent') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.54" color="secondary" size="15px"/>
        </div>
        <div class="linear-progress-title text-accent">Stat X</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.4" color="secondary" size="15px"/>
        </div>
      </div>
    </div>

    <div class="row flex items-stretch home-emustation">
      <div class="col col-md-4 machines-list">
        Nombre de jeux par machines
      </div>
      <div class="col col-md-4 machine">
        Machine en cours ( si oui )
      </div>
      <div class="col col-md-4 game">
        Jeux en cours ( si oui )
      </div>
    </div>
  </q-page>
</template>

<script>
  import {mapState} from 'vuex'
  import Stat from '../components/home/Stat'

  export default {
    name: 'Home',
    components: {Stat},
    created() {
      this.$store.dispatch('monitoring/getStorageInfo')
    },
    computed: {
      ...mapState(['monitoring']),
      stats: state => {
        if (!state.monitoring.storageInfo.recalbox) {
          return []
        }

        let used = state.monitoring.storageInfo.recalbox.core.data.used,
          size = state.monitoring.storageInfo.recalbox.core.data.size

        return [
          {
            key: 1,
            title: 'home.preview.systems',
            value: 84,
            type: 'number',
          },
          {
            key: 2,
            title: 'home.preview.roms',
            value: 70543,
            type: 'number',
          },
          {
            key: 3,
            title: 'home.preview.sharePercent',
            value: used && size ? Math.round((100 * used) / size) : 0,
            type: 'percent',
          },
          {
            key: 4,
            title: 'home.preview.adefinir',
            value: 75,
            type: 'percent',
          },
        ]
      },
    },
    data() {
      return {}
    },
  }
</script>

<style lang="sass">
  .home
    height: 100%

    &:before
      content: "\F56E"

    .home-stats
      background: white
      min-height: 212px
      position: relative

      .preview-title
        text-transform: uppercase
        font-weight: 100
        font-size: x-large
        color: $primary

      .stat
        width: calc((85% / 4) - (1px / 4))

      .home-stat-title
        text-transform: uppercase

      .col-2
        width: calc(16.6667% - (1px / 4))

      .linear-progress-title
        text-transform: uppercase

      .linear-progress-bar
        margin-bottom: .5em

      @media(max-width: 1023px)
        .preview-title
          width: 100%

        .arrow-right,
        .q-separator--vertical
          display: none

      @media(max-width: 900px)
        .stat
          display: none

      @media(min-width: 901px)
        .linear-progress
          display: none

      @media(min-width: 1024px)
        .preview-title
          width: 15%

        .arrow-bottom,
        .q-separator--horizontal
          display: none

    .q-separator--vertical
      background: $rc-light-grey
      width: 2px
      min-width: 2px

    .q-separator--horizontal
      background: $rc-light-grey
      width: 100%
      height: 2px

    .home-emustation
      height: calc(100vh - 212px)

      .machines-list,
      .machine
        border-right: 2px solid white

      .machine
        position: relative
        background: $rc-light-grey

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

      .game
        position: relative
</style>
