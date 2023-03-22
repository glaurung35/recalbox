<template>
  <div class="chart-cpu">
    <apexchart
      :options="options"
      :series="data"
      height="250"
    >
    </apexchart>
  </div>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';
import { date, getCssVar } from 'quasar';
import { useI18n } from 'vue-i18n';

const { t } = useI18n({ useScope: 'global' });

const props = defineProps({
  data: { type: Array, required: true },
});

const { data } = toRefs(props);

const options = computed(() => ({
  chart: {
    type: 'area',
    animations: { enabled: false, easing: 'linear', dynamicAnimation: { speed: 1000 } },
    toolbar: { show: false },
    zoom: { enabled: false },
  },
  dataLabels: {
    enabled: false,
    style: { colors: [getCssVar('primary')] },
    offsetX: 30,
  },
  tooltip: { enabled: false },
  colors: [getCssVar('accent')],
  stroke: {
    curve: 'smooth', show: true, width: 1, colors: [getCssVar('primary')],
  },
  fill: { type: 'gradient', gradient: { opacityFrom: 0.6, opacityTo: 0.8 } },
  markers: { size: 0 },
  yaxis: {
    min: 0,
    max: 100,
    decimalsInFloat: 0,
    title: {
      text: t('monitoring.cpu.temperature.title'), style: { color: getCssVar('primary') },
    },
  },
  xaxis: {
    labels: {
      formatter(value:number, timestamp:number) {
        return date.formatDate(timestamp, 'mm:ss');
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
  },
}));
</script>

<style lang="sass">
.chart-cpu
  padding: 1em
  background: white
</style>
