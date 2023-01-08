<script lang="ts">
	import { db } from '$lib/stores/database';
	import { lastUpdate, status } from '$lib/stores/iot';
	import { onValue, ref } from 'firebase/database';
	import { onDestroy } from 'svelte';
	import moment from 'moment';

	const off = onValue(ref($db, 'last_update_utc'), (snapshot) => {
		$lastUpdate = snapshot.val() || '';
	});
	let lastUpdateDate: moment.Moment | null = null;
	$: if ($lastUpdate) lastUpdateDate = moment($lastUpdate);

	onDestroy(() => {
		off();
	});
</script>

<div class="text-sm">
	<p>Terakhir Diperbarui:<br />{lastUpdateDate?.format('DD/MM/YYYY hh:mm:ss')}</p>
</div>

<style>
	div {
		position: fixed;
		bottom: 2%;
		right: 2%;
	}
</style>
