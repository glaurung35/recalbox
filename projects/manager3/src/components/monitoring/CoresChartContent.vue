<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <apexchart
    :options="options"
    :series="data"
    height="100%"
  />
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';
import { getCssVar } from 'quasar';
import { useI18n } from 'vue-i18n';

const { t } = useI18n({ useScope: 'global' });

const props = defineProps({
  data: { type: Array, required: true },
});

const { data } = toRefs(props);

const options = computed(() => ({
  chart: {
    type: 'bar',
    toolbar: {
      show: false,
    },
    zoom: { enabled: false },
    height: 'auto',
  },
  xaxis: {
    type: 'category',
  },
  responsive: [
    {
      breakpoint: 950,
      options: {
        chart: {
          height: '250px',
        },
      },
    },
  ],
  yaxis: {
    title: {
      text: t('monitoring.cores.title'),
      style: {
        color: getCssVar('accent'),
      },
    },
    min: 0,
    max: 100,
  },
  colors: [getCssVar('accent')],
  fill: {
    opacity: 0.8,
  },
  legend: {
    show: false,
  },
  tooltip: {
    enabled: false,
  },
  plotOptions: {
    bar: {
      dataLabels: {
        position: 'bottom',
      },
      borderRadius: 5,
      borderRadiusApplication: 'end',
    },
  },
  dataLabels: {
    formatter(value: number) {
      return `${value}%`;
    },
    style: {
      fontSize: '12px',
      colors:
        [getCssVar('primary')],
    },
  },
}));
</script>
