<template>
  <q-page class="background monitoring">
    <div class="chart-cpu">
      <apexchart :options="cpuTempOptions" :series="metrics.temperatures" height="250" type="area"></apexchart>
    </div>
    <div class="charts-container row">
      <div class="disks-usage col">
        <apexchart type="bar" height="350" :options="cpusOptions" :series="metrics.cores"></apexchart>
      </div>
      <div class="disks-usage col">
        <!--        <apexchart type="bar" height="350" :options="usageOptions" :series="usageSeries"></apexchart>-->
      </div>
      <div class="disks-usage col">
        <apexchart type="bar" height="350" :options="usageOptions" :series="usageSeries"></apexchart>
      </div>
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
      }
    },
    data() {
      return {
        cpuTempOptions: {
          chart: {
            type: 'line',
            height: 250,
            animations: { enabled: false, easing: 'linear', dynamicAnimation: { speed: 1000 } },
            toolbar: { show: false },
            zoom: { enabled: false }
          },
          dataLabels: { enabled: false, style: { colors: ['#333'] }, offsetX: 30 },
          tooltip: { enabled: false },
          colors: ['#85d6de'],
          stroke: { curve: 'smooth', show: true, width: 1 },
          fill: { type: 'gradient', gradient: { opacityFrom: 0.6, opacityTo: 0.8 } },
          markers: { size: 3 },
          yaxis: { min: 0, max: 100, decimalsInFloat: 0, title: { text: 'CPU °C', style: { color: '#34495e' } }
          },
          xaxis: {
            labels: {
              formatter: function (value, timestamp) {
                return date.formatDate(timestamp, 'mm:ss')
              }
            },
            tooltip: { enabled: false }
          }
        },
        // cpusSeries: [
        //   { name: 'Utilisé', data: [{ x: 'Core 1', y: 84 }, { x: 'Core 2', y: 55 }] },
        //   { name: 'Libre', data: [{ x: 'Core 1', y: 16 }, { x: 'Core 2', y: 45 }] }
        // ],
        usageSeries: [
          { name: 'Utilisé', data: [84, 55, 41] },
          { name: 'Libre', data: [16, 45, 59] }
        ],
        usageOptions: {
          chart: { type: 'bar', height: 350, stacked: true, stackType: '100%' },
          responsive: [{
            breakpoint: 480,
            options: { legend: { position: 'bottom', offsetX: -10, offsetY: 0 } }
          }],
          xaxis: { categories: ['/dev/sda1', '/dev/root', '/dev/mmcblk0p1'] },
          yaxis: { title: { text: 'FREE SPACE', style: { color: '#34495e' } } },
          colors: ['#34495e','#85d6de'],
          fill: { opacity: 0.8 },
          legend: { show: false }
        },
        cpusOptions: {
          chart: { type: 'bar', height: 350, stacked: true, stackType: '100%' },
          responsive: [{
            breakpoint: 480,
            options: { legend: { position: 'bottom', offsetX: -10, offsetY: 0 } }
          }],
          xaxis: { type: 'category' },
          yaxis: { title: { text: 'CPU CORES USAGE', style: { color: '#34495e' } } },
          colors: ['#34495e','#85d6de'],
          fill: { opacity: 0.8 },
          legend: { show: false }
        }
      }
    }
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
</style>
