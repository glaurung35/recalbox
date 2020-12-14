<template>
  <q-page class="background monitoring">
    <div class="chart-cpu">
      <apexchart :options="cpuTempOptions" :series="metrics.temperatures" height="250" type="area"></apexchart>
    </div>
    <div class="charts-container row">
      <div class="cores-usage col">
        <apexchart type="bar" height="350" :options="cpusOptions" :series="metrics.cores"></apexchart>
      </div>
      <div class="disks-usage col">
        <q-linear-progress size="25px" :value="0.35" color="accent">
          <div class="absolute-full flex flex-center">
            <q-badge color="white" text-color="primary" :label="progressLabel" />
          </div>
        </q-linear-progress>
      </div>
<!--      <div class="disks-usage col">-->
<!--        <apexchart type="bar" height="350" :options="usageOptions" :series="usageSeries"></apexchart>-->
<!--      </div>-->
    </div>
  </q-page>
</template>

<script>
  import VueApexCharts from 'vue-apexcharts'
  import { date } from 'quasar'

  export default {
    name: 'Monitoring',
    components: {
      apexchart: VueApexCharts,
    },
    computed: {
      progressLabel () {
        return '/sda1/share | ' + (0.35 * 100).toFixed(0) + '%'
      },
      metrics: {
        get: function () {
          return this.$store.getters['monitoring/metrics'] ? this.$store.getters['monitoring/metrics'] : {
            cores: [
              {name: 'Utilisé', data: [
                {x: 'Core 1', y: 0}, {x: 'Core 2', y: 0}, {x: 'Core 3', y: 0}, {x: 'Core 4', y: 0}
                ]},
              {name: 'Libre', data: [
                {x: 'Core 1', y: 100}, {x: 'Core 2', y: 100}, {x: 'Core 3', y: 100}, {x: 'Core 4', y: 100}
                ]}
            ]
          }
        }
      },
    },
    data() {
      return {
        cpuTempOptions: {
          chart: {
            type: 'line',
            height: 250,
            animations: { enabled: false, easing: 'linear', dynamicAnimation: { speed: 1000 } },
            toolbar: { show: false },
            zoom: { enabled: false },
          },
          dataLabels: { enabled: false, style: { colors: ['#34495e'] }, offsetX: 30 },
          tooltip: { enabled: false },
          colors: ['#85d6de'],
          stroke: { curve: 'smooth', show: true, width: 1, colors: ['#34495e'] },
          fill: { type: 'gradient', gradient: { opacityFrom: 0.6, opacityTo: 0.8 } },
          markers: { size: 0 },
          yaxis: { min: 0, max: 100, decimalsInFloat: 0, title: { text: 'CPU °C ( last 30s )', style: { color: '#34495e' } },
          },
          xaxis: {
            labels: {
              formatter: function (value, timestamp) {
                return date.formatDate(timestamp, 'mm:ss')
              },
            },
            tooltip: { enabled: false },
          },
          grid: {
            show: true,
            strokeDashArray: 0,
            position: 'back',
            xaxis: { lines: { show: true } },
            yaxis: { lines: { show: true } },
            row: { colors: ['#ffffff'], opacity: 0.5 },
            column: { colors: ['#ffffff'], opacity: 0.5 },
          }
        },
        usageSeries: [
          { name: 'Utilisé', data: [84, 55, 41] },
          { name: 'Libre', data: [16, 45, 59] },
        ],
        usageOptions: {
          chart: { type: 'bar', height: 350, stacked: true, stackType: '100%' },
          responsive: [{ breakpoint: 480 }],
          xaxis: { categories: ['/dev/sda1', '/dev/root', '/dev/mmcblk0p1'] },
          yaxis: { title: { text: 'FREE SPACE', style: { color: '#34495e' } } },
          colors: ['#34495e','#85d6de'],
          fill: { opacity: 0.8 },
          legend: { show: false },
        },
        cpusOptions: {
          chart: { type: 'bar', height: 350, toolbar: { show: false } },
          responsive: [{ breakpoint: 480 }],
          xaxis: { type: 'category' },
          yaxis: {
            title: { text: 'CPU CORES USAGE', style: { color: '#34495e' } },
            min: 0,
            max: 100,
          },
          colors: ['#85d6de'],
          fill: { opacity: 0.8 },
          legend: { show: false },
          tooltip: { enabled: false },
          plotOptions: { bar: {dataLabels: { position: 'bottom' } } },
          dataLabels: {
            formatter: function (val) {
              return val + "%"
            },
            style: { fontSize: '12px', colors: ["#34495e"] }
          }
        }
      }
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

      .col
        background: white
        margin: 0 8px 1em

      .cores-usage
        flex: 1

      .disks-usage
        flex: 2
        padding: .5em
</style>
