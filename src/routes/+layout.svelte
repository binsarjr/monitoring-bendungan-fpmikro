<script lang="ts">
	import DatabaseReady from '$lib/components/DatabaseReady.svelte';
	import DataGrafikStoreFunction from '$lib/components/firebase-stores/DataGrafikStoreFunction.svelte';
	import KetinggianAirStoreFunction from '$lib/components/firebase-stores/KetinggianAirStoreFunction.svelte';
	import KoneksiPerangkatStoreFunction from '$lib/components/firebase-stores/LastUpdatedStoreFunction.svelte';
	import StatusStoreFunction from '$lib/components/firebase-stores/StatusStoreFunction.svelte';
	import '../app.css';
</script>

<DatabaseReady>
	<DataGrafikStoreFunction />
	<KoneksiPerangkatStoreFunction />
	<KetinggianAirStoreFunction />
	<StatusStoreFunction />
</DatabaseReady>

<slot />
<div class="h-[10vh]"></div>
