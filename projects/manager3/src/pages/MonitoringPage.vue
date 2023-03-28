<template>
  <q-page class="background monitoring">
    <CPUChartContent :data="JSON.parse(JSON.stringify(areaChartData))"/>
    <div class="charts-container row">
      <CoresChartContent :data="cores"/>
      <StoragesChartContent :data="storages"/>
    </div>
  </q-page>
</template>

<script lang="ts" setup>
import { useMonitoringStore } from 'stores/monitoring';
import { storeToRefs } from 'pinia';
import { computed } from 'vue';
import CPUChartContent from 'components/monitoring/CPUChartContent.vue';
import CoresChartContent from 'components/monitoring/CoresChartContent.vue';
import StoragesChartContent from 'components/monitoring/StoragesChartContent.vue';

const monitoringStore = useMonitoringStore();
const { metrics, monitoring } = storeToRefs(monitoringStore);

monitoringStore.fetch();

const cores = computed<object[]>(() => [{
  data: metrics.value.cores,
}]);
const areaChartData = computed<object[]>(() => [
  { data: metrics.value.temperatures },
  { data: metrics.value.memory },
]);
const storages = computed(() => monitoring.value.storages);
</script>

<style lang="sass">
.monitoring
  position: relative

  &:before
    content: "\F076A"

  .charts-container
    position: relative
    padding: 16px 8px

    .col
      background: white
      margin: 0 8px 1em
</style>
