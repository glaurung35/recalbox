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
        <q-linear-progress size="25px" :value="0.35" color="accent">
          <div class="absolute-full flex flex-center">
            <q-badge color="white" text-color="primary" :label="progressLabel"/>
          </div>
        </q-linear-progress>
      </div>
    </div>
  </q-page>
</template>

<script>
import { date } from 'quasar'
import VueApexCharts from 'vue-apexcharts'
import { mapGetters } from 'vuex'

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
    progressLabel() {
      return '/sda1/share | ' + (0.35 * 100).toFixed(0) + '%'
    }
  },
  data() {
    return {
      temperatureOptions: {
        chart: {
          type: 'line',
          height: 250,
          animations: {enabled: false, easing: 'linear', dynamicAnimation: {speed: 1000}},
          toolbar: {show: false},
          zoom: {enabled: false},
        },
        dataLabels: {enabled: false, style: {colors: ['#34495e']}, offsetX: 30},
        tooltip: {enabled: false},
        colors: ['#85d6de'],
        stroke: {curve: 'smooth', show: true, width: 1, colors: ['#34495e']},
        fill: {type: 'gradient', gradient: {opacityFrom: 0.6, opacityTo: 0.8}},
        markers: {size: 0},
        yaxis: {
          min: 0, max: 100, decimalsInFloat: 0, title: {text: 'CPU °C ( last 30s )', style: {color: '#34495e'}},
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
          title: {text: 'CPU CORES USAGE', style: {color: '#34495e'}},
          min: 0,
          max: 100,
        },
        colors: ['#85d6de'],
        fill: {opacity: 0.8},
        legend: {show: false},
        tooltip: {enabled: false},
        plotOptions: {bar: {dataLabels: {position: 'bottom'}}},
        dataLabels: {
          formatter: function (val) {
            return val + "%"
          },
          style: {fontSize: '12px', colors: ["#34495e"]},
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
      flex: 2
      padding: .5em
</style>
