<script>
	import DataGrafik from '../lib/components/DataGrafik.svelte';
	import StatusBendungan from '../lib/components/StatusBendungan.svelte';
	import { ketinggianAir } from '../lib/stores/iot';
</script>

<main class="px-4 py-2 md:container">
	<div class="flex flex-col gap justify-center items-center">
		<StatusBendungan />
		<div>Ketinggian Air Saat ini {$ketinggianAir}</div>
		<DataGrafik />
	</div>
</main>
