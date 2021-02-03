<template>
  <q-page class="background monitoring">
    <div class="chart-cpu">
      <apexchart :options="temperatureOptions" :series="temperatures" height="250" type="area"></apexchart>
    </div>
    <div class="charts-container row">
      <div class="cores-usage col">
        <apexchart :options="coresOptions" :series="cores" height="350" type="bar"></apexchart>
      </div>
      <div class="disks-usage col">
        <q-linear-progress
          v-for="storage in storages"
          size="30px"
          :value="storage.used"
          color="accent"
          class="q-mt-sm progress"
          :key="storage.label"
          track-color="white"
        >
          <div class="absolute-full flex items-center">
            <q-icon :name="storage.icon" color="white"/>
            <div class="progress-text">
              <span>{{ storage.deviceModel }} | <b>{{ storage.type }}</b> | {{ (storage.used * 100).toFixed(0) }}%</span>
            </div>
          </div>
        </q-linear-progress>
      </div>
    </div>
  </q-page>
</template>

<script>
import { date, colors } from 'quasar'
import VueApexCharts from 'vue-apexcharts'
import {mapGetters, mapState} from 'vuex'

export default {
  name: 'Monitoring',
  components: {
    apexchart: VueApexCharts,
  },
  created() {
    this.$store.dispatch('monitoring/getStorageInfo')
  },
  computed: {
    ...mapGetters({
      cores: 'monitoring/cores',
      temperatures: 'monitoring/temperatures'
    }),
  },
  data() {
    return {
      storages: {
        boot: {
          icon: 'mdi-micro-sd',
          deviceModel: 'SL32G',
          type: 'boot',
          used: 0.34
        },
        overlay: {
          icon: 'mdi-micro-sd',
          deviceModel: 'SL32G',
          type: 'overlay',
          used: .56
        },
        share: {
          icon: 'mdi-usb-flash-drive',
          deviceModel: 'SanDisk Extreme Pro',
          type: 'share',
          used: .75
        },
        network: {
          icon: 'mdi-server-network',
          deviceModel: 'Test',
          type: 'test',
          used: .45
        },
      },
      temperatureOptions: {
        chart: {
          type: 'line',
          height: 250,
          animations: {enabled: false, easing: 'linear', dynamicAnimation: {speed: 1000}},
          toolbar: {show: false},
          zoom: {enabled: false},
        },
        dataLabels: {enabled: false, style: {colors: [colors.getBrand('primary')]}, offsetX: 30},
        tooltip: {enabled: false},
        colors: [colors.getBrand('accent')],
        stroke: {curve: 'smooth', show: true, width: 1, colors: [colors.getBrand('primary')]},
        fill: {type: 'gradient', gradient: {opacityFrom: 0.6, opacityTo: 0.8}},
        markers: {size: 0},
        yaxis: {
          min: 0, max: 100, decimalsInFloat: 0, title: {text: 'CPU °C ( last 30s )', style: {color: colors.getBrand('primary')}},
        },
        xaxis: {
          labels: {
            formatter: function (value, timestamp) {
              return date.formatDate(timestamp, 'mm:ss')
            },
          },
          tooltip: {enabled: false},
        },
        grid: {
          show: true,
          strokeDashArray: 0,
          position: 'back',
          xaxis: {lines: {show: true}},
          yaxis: {lines: {show: true}},
          row: {colors: ['#ffffff'], opacity: 0.5},
          column: {colors: ['#ffffff'], opacity: 0.5},
        },
      },
      coresOptions: {
        chart: {type: 'bar', height: 350, toolbar: {show: false}},
        responsive: [{breakpoint: 480}],
        xaxis: {type: 'category'},
        yaxis: {
          title: {text: 'CPU CORES USAGE', style: {color: colors.getBrand('primary')}},
          min: 0,
          max: 100,
        },
        colors: [colors.getBrand('accent')],
        fill: {opacity: 0.8},
        legend: {show: false},
        tooltip: {enabled: false},
        plotOptions: {bar: {dataLabels: {position: 'bottom'}}},
        dataLabels: {
          formatter: function (val) {
            return val + "%"
          },
          style: {fontSize: '12px', colors: [colors.getBrand('primary')]},
        },
      },
    }
  },
}
</script>

<style lang="sass">
.monitoring
  position: relative

  &:before
    content: "\F769"

  .chart-cpu
    padding: 1em
    background: white

  .charts-container
    position: relative
    padding: 16px 8px

    .col
      background: white
      margin: 0 8px 1em

    .cores-usage
      flex: 1

    .disks-usage
      flex: 1
      padding: .5em

      .progress
        border: 1px solid rgba(240, 240, 240, 1)

        .q-linear-progress__model
          opacity: .5

        .progress-text
          height: 100%
          line-height: 28px
          font-size: .4em
          color: $primary

          span
            vertical-align: middle

        i
          font-size: .7em
          margin-left: .2em
          margin-right: .2em
</style>
