<template>
  <q-page class="background monitoring">
    <div class="chart-cpu">
      <apexchart :options="cpuOptions" :series="metrics.temperatures" height="250" type="area"></apexchart>
    </div>
    <div class="charts-container row">
      <div class="disks-usage col">
        <apexchart type="bar" height="350" :options="usageOptions" :series="usageSeries"></apexchart>
      </div>
      <div class="disks-usage col">
<!--        <apexchart type="pie" height="350" :options="fakeOptions" :series="fakeSeries"></apexchart>-->
      </div>
      <div class="disks-usage col">
<!--        <apexchart type="bar" height="350" :options="usageOptions" :series="usageSeries"></apexchart>-->
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
          return this.$store.getters['monitoring/metrics'] ? this.$store.getters['monitoring/metrics'] : []
        }
      }
    },
    methods: {
      generateDayWiseTimeSeries(baseval, count, yrange, cb) {
        let i = 0;
        let series = [];
        while (i < count) {
          let y = Math.floor(Math.random() * (yrange.max - yrange.min + 1)) + yrange.min;

          series.push([baseval, y]);
          baseval += 86400000;
          i++;
        }
        if(cb) cb(series)
        return series;
      },
    },
    data() {
      return {
        cpuOptions: {
          chart: {
            type: 'line',
            height: 250,
            animations: {
              enabled: false,
              easing: 'linear',
              dynamicAnimation: {
                speed: 1000
              }
            },
            toolbar: {
              show: false
            },
            zoom: {
              enabled: false
            }
          },
          dataLabels: {
            enabled: false
          },
          colors: ['#34495e', '#85d6de', '#CED4DC'],
          stroke: {
            curve: 'straight',
            show: true,
            lineCap: 'butt',
            colors: undefined,
            width: 1,
            dashArray: 0,
          },
          fill: {
            type: 'gradient',
            gradient: {
              opacityFrom: 0.6,
              opacityTo: 0.8,
            },
          },
          legend: {
            position: 'top',
            horizontalAlign: 'left',
          },
          markers: {
            size: 0
          },
          xaxis: {
            labels: {
              formatter: function (value, timestamp) {
                return date.formatDate(timestamp, 'HH:mm:ss')
              },
            }
          }
        },
        usageSeries: [{
          name: 'Utilisé',
          data: [84, 55, 41]
        }, {
          name: 'Libre',
          data: [16, 45, 59]
        }],
        usageOptions: {
          chart: {
            type: 'bar',
            height: 350,
            stacked: true,
            stackType: '100%'
          },
          responsive: [{
            breakpoint: 480,
            options: {
              legend: {
                position: 'bottom',
                offsetX: -10,
                offsetY: 0
              }
            }
          }],
          xaxis: {
            categories: ['/dev/sda1', '/dev/root', '/dev/mmcblk0p1'],
          },
          colors: ['#34495e','#85d6de'],
          fill: {
            opacity: 0.8,
          },
          legend: {
            show: false
          }
        },
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
