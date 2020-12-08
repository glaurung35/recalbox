<template>
  <q-page class="background monitoring">
    <div class="chart-cpu">
      <apexchart :options="cpuOptions" :series="cpuSeries" height="250" type="area"></apexchart>
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

  export default {
    name: 'Monitoring',
    components: {
      apexchart: VueApexCharts,
    },
    methods: {
      generateDayWiseTimeSeries(baseval, count, yrange) {
        let i = 0;
        let series = [];
        while (i < count) {
          let y = Math.floor(Math.random() * (yrange.max - yrange.min + 1)) + yrange.min;

          series.push([baseval, y]);
          baseval += 86400000;
          i++;
        }
        return series;
      },
    },
    data() {
      return {
        cpuSeries: [
          {
            name: 'CPU',
            data: this.generateDayWiseTimeSeries(new Date('11 Feb 2017 GMT').getTime(), 24, {
              min: 10,
              max: 60,
            }),
          },
          {
            name: 'C°',
            data: this.generateDayWiseTimeSeries(new Date('11 Feb 2017 GMT').getTime(), 24, {
              min: 10,
              max: 20,
            }),
          },
          {
            name: 'RAM',
            data: this.generateDayWiseTimeSeries(new Date('11 Feb 2017 GMT').getTime(), 24, {
              min: 10,
              max: 15,
            }),
          },
        ],
        cpuOptions: {
          chart: {
            type: 'area',
            height: 250,
            stacked: true,
            events: {
              selection: function (chart, e) {
                console.log(new Date(e.xaxis.min))
              },
            },
          },
          colors: ['#34495e', '#85d6de', '#CED4DC'],
          dataLabels: {
            enabled: false,
          },
          stroke: {
            show: true,
            curve: 'straight',
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
          xaxis: {
            type: 'datetime',
          },
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
